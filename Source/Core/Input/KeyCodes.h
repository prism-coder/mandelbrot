#pragma once

/**
 * Enumeration of key codes representing all keys on the keyboard based on the GLFW library.
 * Includes printable keys, alphanumeric characters, function keys, special keys, keypad keys,
 * modifier keys, and arrow keys.
 * 
 * The key codes can be used to query the state of keys in the `Input` class, allowing for input
 * handling in applications such as games or interactive software.
 */
enum class KeyCode {
	// Unknown

	Unknown      = -1,

	// Printable Keys

	Space        = 32,
	Apostrophe   = 39,  // '
	Comma        = 44,  // ,
	Minus        = 45,  // -
	Period       = 46,  // .
	Slash        = 47,  // /

	// Numeric Characters

	D0 = 48, D1, D2, D3, D4, D5, D6, D7, D8, D9,

	Semicolon    = 59,  // ;
	Equal        = 61,  // =

	// Alphabetic Characters

	A = 65, B, C, D, E, F, G, H, I, J, K, L, M,
	N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

	LeftBracket  = 91,  // [
	Backslash    = 92,  // '\'
	RightBracket = 93,  // ]
	GraveAccent  = 96,  // `
	World1       = 161, // Non-US #1
	World2       = 162, // Non-US #2

	// Special Keys

	Escape       = 256,
	Enter        = 257,
	Tab          = 258,
	Backspace    = 259,
	Insert       = 260,
	Delete       = 261,

	// Arrow Keys

	Right        = 262,
	Left         = 263,
	Down         = 264,
	Up           = 265,

	// Navigation Keys

	PageUp       = 266,
	PageDown     = 267,
	Home         = 268,
	End          = 269,

	// Lock Keys

	CapsLock     = 280,
	ScrollLock   = 281,
	NumLock      = 282,
	PrintScreen  = 283,
	Pause        = 284,

	// Function Keys

	F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,

	// Keypad Keys

	KP0 = 320, KP1, KP2, KP3, KP4, KP5, KP6, KP7, KP8, KP9,
	KPDecimal    = 330,
	KPDivide     = 331,
	KPMultiply   = 332,
	KPSubtract   = 333,
	KPAdd        = 334,
	KPEnter      = 335,
	KPEqual      = 336,

	// Modifier Keys

	LeftShift    = 340,
	LeftControl  = 341,
	LeftAlt      = 342,
	LeftSuper    = 343,
	RightShift   = 344,
	RightControl = 345,
	RightAlt     = 346,
	RightSuper   = 347,
	Menu         = 348,

	// Limits
	
	Last         = Menu
};