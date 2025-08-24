/* tray_icon.hpp - system tray management header.
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
#include <string>
#include <windows.h>

class tray_icon {
public:
	tray_icon(HWND hwnd, HICON icon, const std::string& message);
	~tray_icon();
	tray_icon(const tray_icon&) = delete;
	tray_icon& operator=(const tray_icon&) = delete;
	tray_icon(tray_icon&&) = delete;
	tray_icon& operator=(tray_icon&&) = delete;

private:
	HWND hwnd;
};
