#include "app.hpp"
#include <memory>
#include <stdexcept>

app::app(HINSTANCE hInstance) :instance{hInstance}, monitor{std::make_unique<mic_monitor>()} {
	WNDCLASS wc{
		.lpfnWndProc = wnd_proc,
		.hInstance = instance,
		.lpszClassName = "MicStateWNDClass",
	};
	RegisterClass(&wc);
	hwnd = CreateWindow(wc.lpszClassName, nullptr, 0, 0, 0, 0, 0, nullptr, nullptr, hInstance, this);
	if (!hwnd) throw std::runtime_error{"Failed to create window."};
	icon = std::make_unique<tray_icon>(hwnd, LoadIcon(nullptr, IDI_APPLICATION), "MicState");
	if (!RegisterHotKey(hwnd, ID_CHECK_HOTKEY, MOD_CONTROL | MOD_ALT | MOD_SHIFT, 'M')) throw std::runtime_error{"Failed to register hotkey."};
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
	return 0;
}

LRESULT CALLBACK app::wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	if (msg == WM_CREATE) {
		auto create_struct = reinterpret_cast<LPCREATESTRUCT>(lp);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create_struct->lpCreateParams));
	}
	auto the_app = reinterpret_cast<app*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
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
		default:
			return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}

void app::show_tray_menu() {
	auto h_menu = std::unique_ptr<std::remove_pointer_t<HMENU>, decltype(&DestroyMenu)>(CreatePopupMenu(), DestroyMenu);
	AppendMenu(h_menu.get(), MF_STRING, ID_EXIT, "Exit");
	POINT pt;
	GetCursorPos(&pt);
	SetForegroundWindow(hwnd);
	TrackPopupMenu(h_menu.get(), TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, nullptr);
}
