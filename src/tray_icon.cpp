#include "app.hPP"
#include "tray_icon.hPP"

tray_icon::tray_icon(HWND hwnd, HICON icon, const std::string& message) :hwnd{hwnd} {
	NOTIFYICONDATA nid{
		.cbSize = sizeof(NOTIFYICONDATA),
		.hWnd = hwnd,
		.uID = ID_TRAY_ICON,
		.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP,
		.uCallbackMessage = WM_TRAYICON,
		.hIcon = icon,
	};
	strncpy_s(nid.szTip, message.data(), sizeof(nid.szTip) - 1);
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
