#pragma once

#include <memory>
#include "mic_monitor.hpp"
#include "tray_icon.hpp"
#include <windows.h>

constexpr UINT ID_TRAY_ICON = 100;
constexpr UINT ID_EXIT = 101;
constexpr UINT ID_CHECK_HOTKEY = 102;
constexpr UINT WM_TRAYICON = WM_USER + 1;

class app {
public:
	app(HINSTANCE hInstance);
	~app();
	int run();
	void show_tray_menu();

private:
	HINSTANCE instance;
	HWND hwnd;
	std::unique_ptr<mic_monitor> monitor;
	std::unique_ptr<tray_icon> icon;
	static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
};
