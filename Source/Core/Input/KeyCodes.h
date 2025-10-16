#pragma once

// Key codes based on the GLFW library
enum class KeyCode {
	// Alphanumeric Characters
	A = 65, B, C, D, E, F, G, H, I, J, K, L, M,
	N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

	// Numeric Characters
	D0 = 48, D1, D2, D3, D4, D5, D6, D7, D8, D9,

	// Function Keys
	F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

	// Special Keys
	Space = 32,
	Escape = 256,
	Enter = 257,
	Tab = 258,
	Backspace = 259,
	LeftShift = 340,
	LeftControl = 341,
	LeftAlt = 342,
	RightControl = 343,
	RightShift = 344,
	RightAlt = 345,
	Insert = 260,
	Delete = 261,
	Home = 268,
	End = 269,
	PageUp = 266,
	PageDown = 267,

	// Arrow Keys
	Right = 262,
	Left = 263,
	Down = 264,
	Up = 265
};

// Input actions
enum class InputAction {
	Unknown = -1,
	Release = 0,
	Press = 1,
	Repeat = 2
};