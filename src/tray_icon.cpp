#include "app.hpp"
#include "tray_icon.hpp"

tray_icon::tray_icon(HWND hwnd, HICON icon, const std::string& message) : hwnd{hwnd} {
	NOTIFYICONDATA nid{
		.cbSize = sizeof(NOTIFYICONDATA),
		.hWnd = hwnd,
		.uID = ID_TRAY_ICON,
		.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP,
		.uCallbackMessage = WM_TRAYICON,
		.hIcon = icon,
	};
	std::strncpy(nid.szTip, message.c_str(), sizeof(nid.szTip) - 1);
	nid.szTip[sizeof(nid.szTip) - 1] = '\0';
	Shell_NotifyIcon(NIM_ADD, &nid);
}

tray_icon::~tray_icon() {
	NOTIFYICONDATA nid{
		.cbSize = sizeof(NOTIFYICONDATA),
		.hWnd = hwnd,
		.uID = ID_TRAY_ICON,
	};
	Shell_NotifyIcon(NIM_DELETE, &nid);
}
