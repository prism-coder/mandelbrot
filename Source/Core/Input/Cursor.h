#pragma once

#include <GLFW/glfw3.h>

/**
 * The `Cursor` class provides functionality for managing the cursor state within a GLFW window.
 * It allows you to lock and unlock the cursor, as well as check if the cursor is currently locked.
 * 
 * The class maintains a reference to the GLFW window and tracks the cursor's lock state.
 */
class Cursor {
public:
	/**
	 * Initializes the cursor management system with the specified GLFW window.
	 * 
	 * This function must be called before using any other cursor-related functions.
	 * 
	 * @param window A pointer to the GLFW window for which the cursor will be managed.
	 */
	static void Init(GLFWwindow* window);

	/// @brief Shuts down the cursor management system, releasing any resources and resetting the state.
	static void Shutdown();

	/// @brief Locks the cursor, preventing it from moving outside the window and hiding it.
	static void Lock();

	/// @brief Unlocks the cursor, allowing it to move freely and making it visible again.
	static void Unlock();

	/// @brief Checks if the cursor is currently locked.
	/// @return `true` if the cursor is locked, `false` otherwise.
	static bool IsLocked();
private:
	Cursor() = default;

	/// @brief A pointer to the GLFW window associated with the cursor management.
	static inline GLFWwindow* s_WindowHandle = nullptr;

	/// @brief A flag indicating whether the cursor is currently locked or not.
	static inline bool s_IsLocked = false;
};