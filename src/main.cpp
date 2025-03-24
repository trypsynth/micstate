#include "app.h"
#include <stdexcept>
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, int) {
	try {
		app app(hInstance);
		return app.run();
	} catch (const std::exception& e) {
		MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
		return 1;
	}
}
