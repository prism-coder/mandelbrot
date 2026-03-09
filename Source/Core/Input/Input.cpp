#include "Input.h"

#include "Core/Log.h"

#include <cstring>

void Input::OnUpdate() {
	// Copy the current key and mouse button states to the previous state arrays to prepare for the next frame's input processing.
	memcpy(s_PreviousKeyState, s_CurrentKeyState, sizeof(s_CurrentKeyState));
	memcpy(s_PreviousMouseButtonState, s_CurrentMouseButtonState, sizeof(s_CurrentMouseButtonState));

	// Calculate the change in mouse position and scroll offset since the last frame.
	CalculateScrollDelta();
	CalculateMouseDelta();
}

bool Input::IsKeyPressed(KeyCode key) {
	return s_CurrentKeyState[static_cast<int32_t>(key)];
}

bool Input::IsKeyDown(KeyCode key) {
	return IsKeyPressed(key) && !s_PreviousKeyState[static_cast<int32_t>(key)];
}

bool Input::IsKeyUp(KeyCode key) {
	return !IsKeyPressed(key) && s_PreviousKeyState[static_cast<int32_t>(key)];
}

bool Input::IsMouseButtonPressed(MouseCode button) {
	return s_CurrentMouseButtonState[static_cast<int32_t>(button)];
}

bool Input::IsMouseButtonDown(MouseCode button) {
	return IsMouseButtonPressed(button) && !s_PreviousMouseButtonState[static_cast<int32_t>(button)];
}

bool Input::IsMouseButtonUp(MouseCode button) {
	return !IsMouseButtonPressed(button) && s_PreviousMouseButtonState[static_cast<int32_t>(button)];
}

bool Input::IsGamepadButtonPressed(GamepadCode button) {
	return s_CurrentGamepadButtonState[static_cast<int32_t>(button)];
}

bool Input::IsGamepadButtonDown(GamepadCode button) {
	return IsGamepadButtonPressed(button) && !s_PreviousGamepadButtonState[static_cast<int32_t>(button)];
}

bool Input::IsGamepadButtonUp(GamepadCode button) {
	return !IsGamepadButtonPressed(button) && s_PreviousGamepadButtonState[static_cast<int32_t>(button)];
}

void Input::SetKeyState(KeyCode key, InputAction action) {
	// Validate the key code before updating the state to prevent out-of-bounds access on the state arrays.
	if (static_cast<int32_t>(key) < 0 || static_cast<int32_t>(key) >= KeyCount) {
		Log::Warning("Input::SetKeyState - Invalid key code: " + std::to_string(static_cast<int32_t>(key)));
		return;
	}

	if (action == InputAction::Press || action == InputAction::Repeat) {
		// Set the current key state to true if the action is Press or Repeat, indicating that the key is currently pressed.
		s_CurrentKeyState[static_cast<int32_t>(key)] = true;
	} else {
		// Set the current key state to false if the action is Release, indicating that the key is currently released.
		s_CurrentKeyState[static_cast<int32_t>(key)] = false;
	}
}

void Input::SetMouseButtonState(MouseCode button, InputAction action) {
	// Validate the mouse button code before updating the state to prevent out-of-bounds access on the state arrays.
	if (static_cast<int32_t>(button) < 0 || static_cast<int32_t>(button) >= MouseButtonCount) {
		Log::Warning("Input::SetMouseButtonState - Invalid mouse button code: " + std::to_string(static_cast<int32_t>(button)));
		return;
	}

	if (action == InputAction::Press || action == InputAction::Repeat) {
		// Set the current mouse button state to true if the action is Press or Repeat, indicating that the mouse button is currently pressed.
		s_CurrentMouseButtonState[static_cast<int32_t>(button)] = true;
	} else {
		// Set the current mouse button state to false if the action is Release, indicating that the mouse button is currently released.
		s_CurrentMouseButtonState[static_cast<int32_t>(button)] = false;
	}
}

void Input::SetMousePosition(double x, double y) {
	// Update the current mouse position with the provided x and y coordinates, converting them to float for storage in the `glm::vec2` variable.
	s_MousePosition = { (float)x, (float)y };
}

void Input::SetScrollOffset(double x, double y) {
	// Update the current scroll offset with the provided x and y coordinates, converting them to float for storage in the `glm::vec2` variable.
	s_ScrollOffset = { (float)x, (float)y };
}

void Input::SetGamepadButtonState(GamepadCode button, InputAction action) {
	// Validate the gamepad button code before updating the state to prevent out-of-bounds access on the state arrays.
	if (static_cast<int32_t>(button) < 0 || static_cast<int32_t>(button) >= GamepadButtonCount) {
		Log::Warning("Input::SetGamepadButtonState - Invalid gamepad button code: " + std::to_string(static_cast<int32_t>(button)));
		return;
	}

	if (action == InputAction::Press || action == InputAction::Repeat) {
		// Set the current gamepad button state to true if the action is Press or Repeat, indicating that the gamepad button is currently pressed.
		s_CurrentGamepadButtonState[static_cast<int32_t>(button)] = true;
	} else {
		// Set the current gamepad button state to false if the action is Release, indicating that the gamepad button is currently released.
		s_CurrentGamepadButtonState[static_cast<int32_t>(button)] = false;
	}
}

void Input::SetGamepadAxisState(GamepadAxis axis, float value) {
	
}

void Input::CalculateMouseDelta() {
	// Calculate the change in mouse position since the last frame by subtracting the previous mouse position from the current mouse position.
	s_MouseDelta = s_MousePosition - s_PreviousMousePosition;
	s_PreviousMousePosition = s_MousePosition;
}

void Input::CalculateScrollDelta() {
	// Calculate the change in scroll offset since the last frame by subtracting the previous scroll offset from the current scroll offset.
	s_ScrollDelta = s_ScrollOffset - s_PreviousScrollOffset;
	s_PreviousScrollOffset = s_ScrollOffset;

	// Reset the current scroll offset to zero after calculating the delta to ensure that scroll input is only registered for the frame in which it occurs.
	s_ScrollOffset = { 0.0f, 0.0f };
}
