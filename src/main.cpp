#include "app.hpp"
#include <stdexcept>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, PSTR, int) {
	try {
		app app(instance);
		return app.run();
	} catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Error", MB_ICONERROR);
		return 1;
	}
}
