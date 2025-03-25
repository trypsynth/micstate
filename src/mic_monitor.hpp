#pragma once

#include <miniaudio.h>
#include <optional>

class mic_monitor {
public:
	mic_monitor();
	~mic_monitor();
	void check_mic_state() const;

private:
	float linear_to_decibel(float linear) const;
	std::optional<int> load_config();
	ma_context context;
	ma_device device;
	mutable float last_level;
	int min_level;
	static void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count);
};
