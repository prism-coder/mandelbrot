#include "Input.h"

#include <cstring>

bool Input::s_CurrentKeyState[KeyCount] = { false };
bool Input::s_PreviousKeyState[KeyCount] = { false };
bool Input::s_CurrentMouseButtonState[MouseButtonCount] = { false };
bool Input::s_PreviousMouseButtonState[MouseButtonCount] = { false };
glm::vec2 Input::s_MousePosition = { 0.0f, 0.0f };

void Input::OnUpdate() {
	memcpy(s_PreviousKeyState, s_CurrentKeyState, sizeof(s_CurrentKeyState));
	memcpy(s_PreviousMouseButtonState, s_CurrentMouseButtonState, sizeof(s_CurrentMouseButtonState));
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

glm::vec2 Input::GetMousePosition() {
	return s_MousePosition;
}

void Input::SetKeyState(KeyCode key, InputAction action) {
	if (action == InputAction::Press || action == InputAction::Repeat) {
		s_CurrentKeyState[static_cast<int32_t>(key)] = true;
	} else {
		s_CurrentKeyState[static_cast<int32_t>(key)] = false;
	}
}

void Input::SetMouseButtonState(MouseCode button, InputAction action) {
	if (action == InputAction::Press || action == InputAction::Repeat) {
		s_CurrentMouseButtonState[static_cast<int32_t>(button)] = true;
	} else {
		s_CurrentMouseButtonState[static_cast<int32_t>(button)] = false;
	}
}

void Input::SetMousePosition(double x, double y) {
	s_MousePosition = { (float)x, (float)y };
}