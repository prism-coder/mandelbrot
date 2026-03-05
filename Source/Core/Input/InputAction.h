#pragma once

/**
 * Enumeration of input actions representing the state of a key or mouse button, including press, release, and repeat states.
 * The input actions are based on the GLFW library, which provides a standardized set of input actions.
 */
enum class InputAction {
	Unknown = -1,
	Release = 0,
	Press = 1,
	Repeat = 2
};