/* mic_monitor.cpp - Microphone state monitoring code.
 *
 * Copyright (C) 2024-2025 Quin Gillespie.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it freely,
 * subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *	claim that you wrote the original software. If you use this software
 *	in a product, an acknowledgment in the product documentation would be
 *	appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *	misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#define NOMINMAX
#include "mic_monitor.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <UniversalSpeech.h>
#include <windows.h>

namespace fs = std::filesystem;

namespace {
	constexpr int DEFAULT_MIN_LEVEL = -60;
	constexpr int SAMPLE_DELAY_MS = 50;
}

mic_monitor::mic_monitor() : last_level{0.0f} {
	if (ma_context_init(nullptr, 0, nullptr, &context) != MA_SUCCESS) throw std::runtime_error("Failed to initialize miniaudio context.");
	config = ma_device_config_init(ma_device_type_capture);
	config.capture.format = ma_format_f32;
	config.capture.channels = 1;
	config.sampleRate = 44100;
	config.dataCallback = data_callback;
	config.pUserData = this;
	min_level = load_config().value_or(DEFAULT_MIN_LEVEL);
}

mic_monitor::~mic_monitor() {
	ma_device_uninit(&device);
	ma_context_uninit(&context);
}

void mic_monitor::data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count) {
	auto* self = static_cast<mic_monitor*>(device->pUserData);
	if (!input) return;
	const auto* samples = static_cast<const float*>(input);
	float peak = 0.0f;
	for (ma_uint32 i = 0; i < frame_count; ++i) peak = (std::max)(peak, std::abs(samples[i]));
	self->last_level = peak;
}

void mic_monitor::check_mic_state() {
	timeBeginPeriod(1);
	ma_device device_temp;
	if (ma_device_init(&context, &config, &device_temp) != MA_SUCCESS) {
		timeEndPeriod(1);
		throw std::runtime_error("Failed to initialize miniaudio device.");
	}
	ma_device_start(&device_temp);
	Sleep(SAMPLE_DELAY_MS);
	ma_device_stop(&device_temp);
	ma_device_uninit(&device_temp);
	const int level = static_cast<int>(linear_to_decibel(last_level));
	const std::wstring status = (level <= min_level) ? L"Muted" : L"Unmuted";
	speechSay(status.c_str(), TRUE);
	timeEndPeriod(1);
}

float mic_monitor::linear_to_decibel(float linear) const {
	constexpr float MIN_DB = -144.0f;
	return (linear > 0.0f) ? 20.0f * std::log10(linear) : MIN_DB;
}

std::optional<int> mic_monitor::load_config() {
	char min_level_str[16]{0};
	const fs::path config_path = fs::current_path() / "config.ini";
	if (GetPrivateProfileString("settings", "min_level", nullptr, min_level_str, sizeof(min_level_str), config_path.string().c_str()) > 0) {
		try {
			return std::stoi(min_level_str);
		} catch (const std::exception&) {
			return std::nullopt;
		}
	}
	return std::nullopt;
}
