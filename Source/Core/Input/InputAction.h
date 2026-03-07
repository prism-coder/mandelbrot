#pragma once

/**
 * Enumeration of input actions representing the state of a key or mouse button, including press, release, and repeat states.
 * The input actions are based on the GLFW library, which provides a standardized set of input actions.
 */
enum class InputAction {
	/// @brief An unknown input action, used as a default value when the action cannot be determined.
	Unknown = -1,

	/// @brief The input action representing a key or mouse button release event.
	Release = 0,

	/// @brief The input action representing a key or mouse button press event.
	Press = 1,

	/// @brief The input action representing a key or mouse button repeat event, which occurs when a key is held down and generates multiple press events.
	Repeat = 2
};