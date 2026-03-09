#pragma once

/**
 * Enumeration of mouse button codes representing various buttons on a mouse, including left, right, middle, and additional buttons.
 * The mouse button codes are based on the GLFW library, which provides a standardized set of mouse button codes.
 * 
 * The mouse button codes can be used to query the state of mouse buttons in the `Input` class, allowing for input handling in applications such as games or interactive software.
 */
enum class MouseCode {
	Button1 		= 0,
	Button2 		= 1,
	Button3 		= 2,
	Button4 		= 3,
	Button5 		= 4,
	Button6 		= 5,
	Button7 		= 6,
	Button8 		= 7,

	ButtonLeft 		= Button1,
	ButtonRight 	= Button2,
	ButtonMiddle 	= Button3,

	// Limits

	Last 			= Button8
};