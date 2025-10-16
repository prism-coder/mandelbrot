#pragma once

#include "Core/Input/KeyCodes.h"
#include "Core/Input/MouseCodes.h"

#include <glm/glm.hpp>

class Input {
public:
	static void OnUpdate();

	static bool IsKeyPressed(KeyCode key);
	static bool IsKeyDown(KeyCode key);
	static bool IsKeyUp(KeyCode key);

	static bool IsMouseButtonPressed(MouseCode button);
	static bool IsMouseButtonDown(MouseCode button);
	static bool IsMouseButtonUp(MouseCode button);
	static glm::vec2 GetMousePosition();

	static void SetKeyState(KeyCode key, InputAction action);
	static void SetMouseButtonState(MouseCode button, InputAction action);
	static void SetMousePosition(double x, double y);
private:
	Input() = default;

	static constexpr size_t KeyCount = 350;
	static constexpr size_t MouseButtonCount = 8;

	static bool s_CurrentKeyState[KeyCount];
	static bool s_PreviousKeyState[KeyCount];

	static bool s_CurrentMouseButtonState[MouseButtonCount];
	static bool s_PreviousMouseButtonState[MouseButtonCount];
	static glm::vec2 s_MousePosition;
};