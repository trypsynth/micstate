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
