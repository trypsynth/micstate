/* app.hpp - Main application header file.
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
#include <memory>
#include "mic_monitor.hpp"
#include "tray_icon.hpp"
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace {
	constexpr UINT ID_TRAY_ICON = 100;
	constexpr UINT ID_EXIT = 101;
	constexpr UINT ID_CHECK_HOTKEY = 102;
	constexpr UINT WM_TRAYICON = WM_USER + 1;
}

class app {
public:
	app(HINSTANCE hInstance);
	~app();
	int run();

private:
	HINSTANCE instance;
	HWND hwnd;
	std::unique_ptr<mic_monitor> monitor;
	std::unique_ptr<tray_icon> icon;

	static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void show_tray_menu();
};
