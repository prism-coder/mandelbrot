#pragma once

extern Application* CreateApplication();

int Main(int argc, char** argv) {
	Application* app = CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#if defined(MB_PLATFORM_WINDOWS) && defined(MB_DIST)

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
	return Main(__argc, __argv);
}

#else

int main(int argc, char** argv) {
	return Main(argc, argv);
}

#endif // defined(MB_PLATFORM_WINDOWS) && defined(MB_DIST)