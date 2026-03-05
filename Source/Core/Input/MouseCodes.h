#pragma once

/**
 * Enumeration of mouse button codes representing various buttons on a mouse, including left, right, middle, and additional buttons.
 * The mouse button codes are based on the GLFW library, which provides a standardized set of mouse button codes.
 * 
 * The mouse button codes can be used to query the state of mouse buttons in the `Input` class, allowing for input handling in applications such as games or interactive software.
 */
enum class MouseCode {
	Button0 = 0,
	Button1 = 1,
	Button2 = 2,
	Button3 = 3,
	Button4 = 4,
	Button5 = 5,
	Button6 = 6,
	Button7 = 7,

	ButtonLast = Button7,
	ButtonLeft = Button0,
	ButtonRight = Button1,
	ButtonMiddle = Button2
};