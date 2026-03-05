#pragma once

#include "Core/Input/KeyCodes.h"
#include "Core/Input/MouseCodes.h"
#include "Core/Input/InputAction.h"

#include <glm/glm.hpp>

/**
 * Input handling class that provides static methods to query the state of keys and mouse buttons, as well as mouse position and scroll offset.
 * It maintains the current and previous state of keys and mouse buttons to allow for detecting key presses, releases, and holds.
 * 
 * The class is designed to be updated every frame by calling the `OnUpdate()` method, which should be called at the end of each frame to update the previous state with the current state.
 */
class Input {
public:
	/**
	 * Updates the input state by copying the current state to the previous state.
	 * 
	 * This method should be called at the end of each frame to ensure that the input states are correctly updated for the next frame.
	 */
	static void OnUpdate();

	/**
	 * Checks if a key is currently pressed.
	 * 
	 * @param key The key to check. This should be a value from the `KeyCode` enumeration.
	 * @return `true` if the key is pressed, `false` otherwise.
	 */
	static bool IsKeyPressed(KeyCode key);

	/**
	 * Checks if a key is currently released.
	 * 
	 * @param key The key to check. This should be a value from the `KeyCode` enumeration.
	 * @return `true` if the key is released, `false` otherwise.
	 */
	static bool IsKeyDown(KeyCode key);

	/**
	 * Checks if a key was released in the current frame.
	 * 
	 * @param key The key to check. This should be a value from the `KeyCode` enumeration.
	 * @return `true` if the key was released in the current frame, `false` otherwise.
	 */
	static bool IsKeyUp(KeyCode key);

	/**
	 * Checks if a mouse button is currently pressed.
	 * 
	 * @param button The mouse button to check. This should be a value from the `MouseCode` enumeration.
	 * @return `true` if the mouse button is pressed, `false` otherwise.
	 */
	static bool IsMouseButtonPressed(MouseCode button);

	/**
	 * Checks if a mouse button is currently held down.
	 * 
	 * @param button The mouse button to check. This should be a value from the `MouseCode` enumeration.
	 * @return `true` if the mouse button is held down, `false` otherwise.
	 */
	static bool IsMouseButtonDown(MouseCode button);

	/**
	 * Checks if a mouse button was released in the current frame.
	 * 
	 * @param button The mouse button to check. This should be a value from the `MouseCode` enumeration.
	 * @return `true` if the mouse button was released in the current frame, `false` otherwise.
	 */
	static bool IsMouseButtonUp(MouseCode button);

	/**
	 * Gets the current mouse position in screen coordinates.
	 * 
	 * @return The current mouse position as a `glm::vec2`.
	 */
	static const glm::vec2 GetMousePosition() { return s_MousePosition; }

	/**
	 * Gets the change in mouse position since the last frame.
	 * 
	 * @return The change in mouse position as a `glm::vec2`.
	 */
	static const glm::vec2 GetMouseDelta() { return s_MouseDelta; }

	/**
	 * Gets the current scroll offset.
	 * 
	 * @return The current scroll offset as a `glm::vec2`.
	 */
	static const glm::vec2 GetScrollOffset() { return s_ScrollOffset; }

	/**
	 * Gets the change in scroll offset since the last frame.
	 * 
	 * @return The change in scroll offset as a `glm::vec2`.
	 */
	static const glm::vec2 GetScrollDelta() { return s_ScrollDelta; }

	/**
	 * Sets the state of a key.
	 * 
	 * This method is intended to be called by the platform-specific input handling code to update the state of keys.
	 * 
	 * @param key The key to update. This should be a value from the `KeyCode` enumeration.
	 * @param action The action representing the new state of the key.
	 * This should be a value from the `InputAction` enumeration, indicating whether the key is pressed, released, or held down.
	 */
	static void SetKeyState(KeyCode key, InputAction action);

	/**
	 * Sets the state of a mouse button.
	 * 
	 * This method is intended to be called by the platform-specific input handling code to update the state of mouse buttons.
	 * 
	 * @param button The mouse button to update. This should be a value from the `MouseCode` enumeration.
	 * @param action The action representing the new state of the mouse button.
	 * This should be a value from the `InputAction` enumeration, indicating whether the button is pressed, released, or held down.
	 */
	static void SetMouseButtonState(MouseCode button, InputAction action);

	/**
	 * Sets the current mouse position.
	 * 
	 * This method is intended to be called by the platform-specific input handling code to update the current mouse position.
	 * 
	 * @param x The x-coordinate of the mouse position in screen coordinates.
	 * @param y The y-coordinate of the mouse position in screen coordinates.
	 */
	static void SetMousePosition(double x, double y);

	/**
	 * Sets the current scroll offset.
	 * 
	 * This method is intended to be called by the platform-specific input handling code to update the current scroll offset.
	 * 
	 * @param x The x-coordinate of the scroll offset.
	 * @param y The y-coordinate of the scroll offset.
	 */
	static void SetScrollOffset(double x, double y);
private:
	Input() = default;

	/**
	 * Calculates the change in mouse position offset since the last frame by comparing the current and previous values.
	 * 
	 * This method is called during the `OnUpdate()` process to ensure that the mouse delta is correctly calculated for the current frame.
	 * The mouse delta is calculated by subtracting the previous mouse position from the current mouse position.
	 */
	static void CalculateMouseDelta();

	/**
	 * Calculates the change in scroll offset since the last frame by comparing the current and previous values.
	 * 
	 * This method is called during the `OnUpdate()` process to ensure that the scroll delta is correctly calculated for the current frame.
	 * The scroll delta is calculated by subtracting the previous scroll offset from the current scroll offset.
	 */
	static void CalculateScrollDelta();

	/**
	 * The maximum number of keys and mouse buttons that can be tracked by the input system.
	 * These values are based on the GLFW library, which defines a maximum of 350 keys.
	 */
	inline static constexpr size_t KeyCount = 350;

	/**
	 * The maximum number of mouse buttons that can be tracked by the input system.
	 * This value is based on the GLFW library, which defines a maximum of 8 mouse buttons.
	 */
	inline static constexpr size_t MouseButtonCount = 8;

	/// @brief Array to store the current state of key buttons.
	inline static bool s_CurrentKeyState[KeyCount] = { false };

	/// @brief Array to store the previous state of key buttons.
	inline static bool s_PreviousKeyState[KeyCount] = { false };

	/// @brief Array to store the current state of mouse buttons.
	inline static bool s_CurrentMouseButtonState[MouseButtonCount] = { false };

	/// @brief Array to store the previous state of mouse buttons.
	inline static bool s_PreviousMouseButtonState[MouseButtonCount] = { false };

	/// @brief  Variable that stores the previous mouse position in screen coordinates.
	inline static glm::vec2 s_PreviousMousePosition = { 0.0f, 0.0f };

	/// @brief Variable that stores the current mouse position in screen coordinates.
	inline static glm::vec2 s_MousePosition = { 0.0f, 0.0f };

	/// @brief Variable that stores the change in mouse position since the last frame.
	inline static glm::vec2 s_MouseDelta = { 0.0f, 0.0f };

	/// @brief Variable that stores the previous scroll offset.
	inline static glm::vec2 s_PreviousScrollOffset = { 0.0f, 0.0f };

	/// @brief Variable that stores the current scroll offset.
	inline static glm::vec2 s_ScrollOffset = { 0.0f, 0.0f };

	/// @brief Variable that stores the change in scroll offset since the last frame.
	inline static glm::vec2 s_ScrollDelta = { 0.0f, 0.0f };
};