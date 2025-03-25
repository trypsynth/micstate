#include "app.hPP"
#include "tray_icon.hPP"

tray_icon::tray_icon(HWND hwnd, UINT id, HICON icon, const std::string& message) :hwnd{hwnd}, id{id} {
	NOTIFYICONDATA nid{};
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = id;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = ID_TRAY_ICON;
	nid.hIcon = icon;
	strncpy_s(nid.szTip, message.data(), sizeof(nid.szTip) - 1);
	Shell_NotifyIcon(NIM_ADD, &nid);
}

tray_icon::~tray_icon() {
	NOTIFYICONDATA nid{};
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = id;
	Shell_NotifyIcon(NIM_DELETE, &nid);
}
