/* app.cpp - Main application implementation.
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

#include "app.hpp"
#include <memory>
#include <stdexcept>

app::app(HINSTANCE inst) : instance{inst}, monitor{std::make_unique<mic_monitor>()} {
	WNDCLASS wc{
		.lpfnWndProc = wnd_proc,
		.hInstance = instance,
		.lpszClassName = "MicStateWNDClass",
	};
	RegisterClass(&wc);
	hwnd = CreateWindow(wc.lpszClassName, nullptr, 0, 0, 0, 0, 0, nullptr, nullptr, instance, this);
	if (!hwnd) throw std::runtime_error("Failed to create window.");
	icon = std::make_unique<tray_icon>(hwnd, LoadIcon(nullptr, IDI_APPLICATION), "MicState");
	if (!RegisterHotKey(hwnd, ID_CHECK_HOTKEY, MOD_CONTROL | MOD_ALT | MOD_SHIFT, 'M')) throw std::runtime_error("Failed to register hotkey.");
}

app::~app() {
	UnregisterHotKey(hwnd, ID_CHECK_HOTKEY);
}

int app::run() {
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return EXIT_SUCCESS;
}

LRESULT CALLBACK app::wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	if (msg == WM_CREATE) {
		const auto create_struct = reinterpret_cast<LPCREATESTRUCT>(lp);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create_struct->lpCreateParams));
	}
	const auto the_app = reinterpret_cast<app*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (!the_app) return DefWindowProc(hwnd, msg, wp, lp);
	switch (msg) {
		case WM_HOTKEY:
			if (wp == ID_CHECK_HOTKEY) the_app->monitor->check_mic_state();
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_TRAYICON:
			if (LOWORD(lp) == WM_RBUTTONUP) the_app->show_tray_menu();
			break;
		case WM_COMMAND:
			if (LOWORD(wp) == ID_EXIT) PostQuitMessage(0);
			break;
		default: return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}

void app::show_tray_menu() {
	const auto h_menu = std::unique_ptr<std::remove_pointer_t<HMENU>, decltype(&DestroyMenu)>(CreatePopupMenu(), DestroyMenu);
	AppendMenu(h_menu.get(), MF_STRING, ID_EXIT, "Exit");
	POINT pt;
	GetCursorPos(&pt);
	SetForegroundWindow(hwnd);
	TrackPopupMenu(h_menu.get(), TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, nullptr);
}
