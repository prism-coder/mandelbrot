#pragma once

#include <GLFW/glfw3.h>

class Cursor {
public:
	static void Init(GLFWwindow* window);
	static void Shutdown();

	static void Lock();
	static void Unlock();
	static bool IsLocked();

private:
	Cursor() = default;

	static GLFWwindow* s_WindowHandle;
	static bool s_IsLocked;
};