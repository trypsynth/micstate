#pragma once

#include <string>
#include <windows.h>

class tray_icon {
public:
	tray_icon(HWND hwnd, HICON icon, const std::string& message);
	~tray_icon();

private:
	HWND hwnd;
};
