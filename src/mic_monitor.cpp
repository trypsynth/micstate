#include <cmath>
#include <filesystem>
#include <functional>
#include <memory>
#include "mic_monitor.hpp"
#include <string>
#include <UniversalSpeech.h>
#include <windows.h>

namespace fs = std::filesystem;

mic_monitor::mic_monitor() : last_level{0.0} {
	if (ma_context_init(nullptr, 0, nullptr, &context) != MA_SUCCESS) throw std::runtime_error{"Failed to initialize miniaudio context."};
	config = ma_device_config_init(ma_device_type_capture);
	config.capture.format = ma_format_f32;
	config.capture.channels = 1;
	config.sampleRate = 44100;
	config.dataCallback = data_callback;
	config.pUserData = this;
	min_level = load_config().value_or(-60);
}

mic_monitor::~mic_monitor() {
	ma_device_uninit(&device);
	ma_context_uninit(&context);
}

void mic_monitor::data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count) {
	mic_monitor* self = static_cast<mic_monitor*>(device->pUserData);
	if (!input) return;
	const float* samples = static_cast<const float*>(input);
	float peak = 0.0f;
	for (ma_uint32 i = 0; i < frame_count; ++i) peak = (std::max)(peak, std::fabs(samples[i]));
	self->last_level = peak;
}

void mic_monitor::check_mic_state() {
	timeBeginPeriod(1);
	ma_device device;
	if (ma_device_init(&context, &config, &device) != MA_SUCCESS) throw std::runtime_error{"Failed to initialize miniaudio device."};
	ma_device_start(&device);
	Sleep(50);
	ma_device_stop(&device);
	ma_device_uninit(&device);
	int level = static_cast<int>(linear_to_decibel(last_level));
	std::wstring status = (level <= min_level) ? L"Muted" : L"Unmuted";
	speechSay(status.c_str(), TRUE);
	timeEndPeriod(1);
}

inline float mic_monitor::linear_to_decibel(float linear) const {
	return (linear > 0.0f) ? 20.0f * log10f(linear) : -144.0f;
}

std::optional<int> mic_monitor::load_config() {
	char min_level_str[16] {0};
	fs::path config_path = fs::current_path() / "config.ini";
	if (GetPrivateProfileString("settings", "min_level", nullptr, min_level_str, sizeof(min_level_str), config_path.string().c_str()) > 0) return std::stoi(min_level_str);
	return std::nullopt;
}
