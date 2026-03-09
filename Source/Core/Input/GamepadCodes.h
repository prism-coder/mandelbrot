#pragma once

/**
 * Enumeration of gamepad codes representing all buttons on a standard gamepad.
 * The button codes are based on the GLFW library, which provides a standardized set of
 * gamepad button codes mapped to an Xbox-style layout.
 * PlayStation aliases (Cross, Circle, Square, Triangle) are also provided.
 *
 * The button codes can be used to query the state of gamepad buttons in the `Input` class,
 * allowing for input handling in applications such as games or interactive software.
 */
enum class GamepadCode {
	// Face Buttons

    A           = 0,
	B           = 1,
	X           = 2,
	Y           = 3,

	// PlayStation Aliases

	Cross       = A,
	Circle      = B,
	Square      = X,
	Triangle    = Y,

	// Shoulder Buttons

	LeftBumper  = 4,
	RightBumper = 5,

	// Menu Buttons

	Back        = 6,
	Start       = 7,
	Guide       = 8,

	// Thumbstick Buttons

	LeftThumb   = 9,
	RightThumb  = 10,

	// D-Pad

	DPadUp      = 11,
	DPadRight   = 12,
	DPadDown    = 13,
	DPadLeft    = 14,

	// Limits

    Last        = DPadLeft
};

/**
 * Enumeration of gamepad axis codes representing all analog axes on a standard gamepad.
 * The axis codes are based on the GLFW library. Axis values range from -1.0 to 1.0,
 * except for triggers which range from -1.0 (released) to 1.0 (fully pressed).
 *
 * The axis codes can be used to query the state of gamepad axes in the `Input` class,
 * allowing for analog input handling in applications such as games or interactive software.
 */
enum class GamepadAxis {
	// Left Thumbstick

	LeftX           = 0,
	LeftY           = 1,

	// Right Thumbstick

	RightX          = 2,
	RightY          = 3,

	// Triggers

	LeftTrigger     = 4,
	RightTrigger    = 5,

	// Limits

	Last        = RightTrigger
};

