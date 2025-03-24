#pragma once

#include <memory>
#include "mic_monitor.h"
#include "tray_icon.h"
#include <windows.h>

constexpr UINT ID_TRAY_ICON = 101;
constexpr UINT ID_EXIT = 102;
constexpr UINT ID_CHECK_HOTKEY = 103;

class app {
public:
	app(HINSTANCE hInstance);
	~app();
	int run();

private:
	HINSTANCE instance;
	HWND hwnd;
	HICON icon;
	std::unique_ptr<mic_monitor> monitor;
	std::unique_ptr<tray_icon> trayicon;
	static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
};
