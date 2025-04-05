#pragma once

#include <miniaudio.h>
#include <optional>

class mic_monitor {
public:
	mic_monitor();
	~mic_monitor();
	void check_mic_state();

private:
	ma_context context;
	ma_device device;
	ma_device_config config;
	mutable float last_level;
	int min_level, sleep_time;

	float linear_to_decibel(float linear) const;
	std::optional<int> load_config();
	static void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count);
};
