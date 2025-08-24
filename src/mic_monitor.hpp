/* mic_monitor.hpp - Microphone state monitoring header file.
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
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#pragma once
#include <miniaudio.h>
#include <optional>

class mic_monitor {
public:
	mic_monitor();
	~mic_monitor();
	mic_monitor(const mic_monitor&) = delete;
	mic_monitor& operator=(const mic_monitor&) = delete;
	mic_monitor(mic_monitor&&) = delete;
	mic_monitor& operator=(mic_monitor&&) = delete;
	void check_mic_state();

private:
	ma_context context;
	ma_device device;
	ma_device_config config;
	mutable float last_level;
	int min_level;

	[[nodiscard]] float linear_to_decibel(float linear) const;
	[[nodiscard]] std::optional<int> load_config();
	static void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count);
};
