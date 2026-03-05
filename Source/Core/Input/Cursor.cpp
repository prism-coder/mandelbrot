#include "Cursor.h"

#include "Core/Log.h"

void Cursor::Init(GLFWwindow* window) {
	Log::Trace("Cursor::Init - Initializing the Cursor");
	s_WindowHandle = window;
}

void Cursor::Shutdown() {
	Log::Trace("Cursor::Shutdown - Shutting down the Cursor");
	s_WindowHandle = nullptr;
}

void Cursor::Lock() {
	if (s_IsLocked) {
		// If the cursor is already locked, we can simply return early to avoid redundant state changes and unnecessary calls to GLFW functions.
		return;
	}

	Log::Trace("Cursor::Lock - Locking the Cursor");

	glfwSetInputMode(s_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	s_IsLocked = true;
}

void Cursor::Unlock() {
	if (!s_IsLocked) {
		// If the cursor is already unlocked, we can simply return early to avoid redundant state changes and unnecessary calls to GLFW functions.
		return;
	}

	Log::Trace("Cursor::Unlock - Unlocking the Cursor");

	glfwSetInputMode(s_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	s_IsLocked = false;
}

bool Cursor::IsLocked() {
	return s_IsLocked;
}