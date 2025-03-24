#include "app.h"
#include <memory>
#include <stdexcept>
#include "tray_icon.h"

app::app(HINSTANCE hInstance) :instance{hInstance}, monitor{std::make_unique<mic_monitor>()} {
	WNDCLASS wc{};
	wc.lpfnWndProc = wnd_proc;
	wc.hInstance = instance;
	wc.lpszClassName = "MicStateWNDClass";
	RegisterClass(&wc);
	hwnd = CreateWindow(wc.lpszClassName, nullptr, 0, 0, 0, 0, 0, nullptr, nullptr, hInstance, this);
	if (!hwnd) throw std::runtime_error{"Failed to create window."};
	icon = LoadIcon(nullptr, IDI_APPLICATION);
	trayicon = std::make_unique<tray_icon>(hwnd, ID_TRAY_ICON, icon, "MicState");
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
		auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lp);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
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
		case WM_COMMAND:
			if (LOWORD(wp) == ID_EXIT) PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}
