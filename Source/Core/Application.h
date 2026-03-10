#pragma once

#include "Core/Core.h"
#include "Core/Layer.h"
#include "Core/Image.h"

#include "Layers/Layers.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
#include <vector>

/**
 * Represents the size of the application window, including width and height in pixels.
 */
struct ScreenSize {
	/// @brief The width of the application window in pixels.
	int Width;

	/// @brief The height of the application window in pixels.
	int Height;
};

/**
 * Represents the different screen modes that the application can run in.
 */
enum class ScreenMode {
	/// @brief The application runs in a standard window with borders and title bar.
	Windowed,

	/// @brief The application runs in fullscreen mode, taking up the entire screen and hiding the taskbar and window borders.
	Fullscreen,

	/// @brief The application runs in a borderless window that covers the entire screen, but does not hide the taskbar.
	Borderless
};

/**
 * Represents the specification for the application, including its name, screen size, screen mode, and various settings such as whether pressing Escape closes the app, whether the window starts maximized, whether VSync is enabled, and whether to log to a file.
 */
struct ApplicationSpecification {
	/// @brief The name of the application, used for the window title and log file name.
	std::string Name = "Application";

	/// @brief The size of the application window, including width and height in pixels.
	ScreenSize ScreenSize = { 1920, 1080 };

	/// @brief The screen mode for the application, which can be `Windowed`, `Fullscreen`, or `Borderless`.
	ScreenMode ScreenMode = ScreenMode::Windowed;

	/// @brief A boolean flag indicating whether pressing the Escape key should close the application.
	bool EscapeClosesApp = false;

	/// @brief A boolean flag indicating whether the application window should start maximized.
	bool Maximized = false;

	/// @brief A boolean flag indicating whether vertical synchronization (VSync) should be enabled.
	bool VSync = true;

	/// @brief A boolean flag indicating whether the application should log messages to a file.
	bool LogToFile = true;

	/// @brief A boolean flag indicating whether to lock the frame rate to the target frame rate specified in `TargetFrameRate`.
	bool LockFramerate = false;

	/// @brief The target frame rate cap in frames per second.
	int TargetFrameRate = 0;
};

/**
 * The main application class that manages the application lifecycle, including initialization, running the main loop, handling layers, and shutting down.
 * It also provides static access to the application instance and the main window handle, as well as methods for setting VSync, changing the window title, and retrieving the application specification.
 * 
 * The `Application` class is designed to be a singleton, with a static instance pointer that can be accessed globally.
 * It manages a stack of layers, which can be used to organize different parts of the application (e.g., `Game Logic`, `UI`, etc.) and provides callback functions for handling input events from GLFW.
 */
class Application {
public:
	/**
	 * Constructor that initializes the application with the given specification.
	 * If no specification is provided, it uses default values.
	 */
	Application(const ApplicationSpecification& spec = {});

	/**
	 * Destructor that cleans up resources used by the application.
	 * This will automatically be called when the application is closed, and will ensure that any resources allocated by the application are properly released.
	 */
	~Application();

	/**
	 * Runs the main application loop, which continues until the application is closed.
	 * 
	 * This method will handle updating layers, rendering, and processing input events.
	 * It will also manage the timing of updates and rendering to ensure a smooth experience.
	 */
	void Run();

	/**
	 * Pushes a new layer onto the layer stack.
	 * 
	 * The layer will be attached to the application and will start receiving update and render calls.
	 * 
	 * @param layer A pointer to the layer to be added to the application. The application will take ownership of the layer and will be responsible for deleting it when the application is closed.
	 */
	void PushLayer(Layer* layer);

	/**
	 * Closes the application, which will cause the main loop to exit and the application to shut down.
	 * 
	 * This method can be called from anywhere in the application to signal that the application should close, such as in response to a user action or an error condition.
	 * It will ensure that the application shuts down gracefully, cleaning up resources and saving any necessary state before exiting.
	 */
	void Close();

	/**
	 * Provides static access to the singleton instance of the application.
	 * 
	 * This method allows other parts of the code to access the application instance without needing to pass it around as a parameter.
	 * It is important to note that this method assumes that the application instance has already been created and will return a reference to it.
	 * If the application instance has not been created yet, this method will result in undefined behavior.
	 */
	static Application& Get() { return *s_Instance; }

	/**
	 * Provides access to the main window handle used by the application, which is a pointer to a `GLFWwindow` object.
	 */
	GLFWwindow* GetWindowHandle() const { return m_WindowHandle; }

	/**
	 * Gets the width of the application window.
	 * 
	 * @return The width of the window in pixels.
	 */
	uint32_t GetWidth() const;

	/**
	 * Gets the height of the application window.
	 * 
	 * @return The height of the window in pixels.
	 */
	uint32_t GetHeight() const;

	/**
	 * Sets whether vertical synchronization (VSync) is enabled for the application.
	 * 
	 * @param enabled A boolean flag indicating whether to enable VSync (`true`) or disable it (`false`).
	 */
	void SetVSync(bool enabled);

	/**
	 * Sets the title of the application window.
	 * 
	 * @param title The new title for the application window.
	 */
	void SetWindowTitle(const std::string& title);

	/**
	 * Retrieves the current application specification, which includes the application name, screen size, screen mode, and various settings.
	 * 
	 * @return A constant reference to the current `ApplicationSpecification` used by the application.
	 */
	const ApplicationSpecification& GetSpecification() const { return m_Specification; }
private:
	/**
	 * Initializes the application, including setting up GLFW, creating the window, and loading resources.
	 */
	void Init();

	/**
	 * Shuts down the application, cleaning up resources and terminating GLFW.
	 */
	void Shutdown();

	/**
	 * Loads the application icon from a specified file path and sets it as the window icon using GLFW.
	 */
	void LoadAppIcon();

	/**
	 * GLFW callback function for handling key input events.
	 * 
	 * This function will be called by GLFW whenever a key event occurs, and it will translate the GLFW key codes and actions into the application's input system.
	 * It will update the state of the keys in the application's input system based on the key code and action received from GLFW.
	 * 
	 * @param window The GLFW window that received the event.
	 * @param key The GLFW key code for the key that was pressed or released.
	 * @param scancode The platform-specific scancode for the key that was pressed or released.
	 * @param action The GLFW action code indicating whether the key was pressed, released, or repeated.
	 * @param mods The bitfield of modifier keys (e.g., `Shift`, `Ctrl`) that were active during the key event.
	 */
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	/**
	 * GLFW callback function for handling mouse button input events.
	 * 
	 * This function will be called by GLFW whenever a mouse button event occurs, and it will translate the GLFW mouse button codes and actions into the application's input system.
	 * It will update the state of the mouse buttons in the application's input system based on the button code and action received from GLFW.
	 * 
	 * @param window The GLFW window that received the event.
	 * @param button The GLFW mouse button code for the button that was pressed or released.
	 * @param action The GLFW action code indicating whether the button was pressed or released.
	 * @param mods The bitfield of modifier keys (e.g., `Shift`, `Ctrl`) that were active during the mouse button event.
	 */
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	/**
	 * GLFW callback function for handling mouse scroll events.
	 * 
	 * This function will be called by GLFW whenever a mouse scroll event occurs, and it will translate the scroll offsets into the application's input system.
	 * It will update the state of the scroll offsets in the application's input system based on the scroll event received from GLFW.
	 * 
	 * @param window The GLFW window that received the event.
	 * @param xoffset The scroll offset along the x-axis.
	 * @param yoffset The scroll offset along the y-axis.
	 */
	static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	/**
	 * GLFW callback function for handling cursor position events.
	 * 
	 * This function will be called by GLFW whenever the cursor moves within the window, and it will translate the cursor position into the application's input system.
	 * It will update the state of the mouse position in the application's input system based on the cursor position received from GLFW.
	 * 
	 * @param window The GLFW window that received the event.
	 * @param xpos The new x-coordinate of the cursor position.
	 * @param ypos The new y-coordinate of the cursor position.
	 */
	static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	/**
	 * GLFW callback function for handling gamepad button input events.
	 * 
	 * This function will be called by GLFW whenever a gamepad button event occurs, and it will translate the GLFW gamepad button codes and actions into the application's input system.
	 * It will update the state of the gamepad buttons in the application's input system based on
	 */
	static void GamepadButtonCallback(GLFWwindow *window, int button, int action, int mods);

	/**
	 * GLFW callback function for handling gamepad axis input events.
	 * 
	 * This function will be called by GLFW whenever a gamepad axis event occurs, and it will translate the GLFW gamepad axis codes and values into the application's input system.
	 * It will update the state of the gamepad axes in the application's input system based on
	 */
	static void GamepadAxisCallback(GLFWwindow *window, int axis, float value);

	/**
	 * GLFW callback function for handling error events.
	 * 
	 * This function will be called by GLFW whenever an error occurs, and it will log the error message using the application's logging system.
	 * It will translate the GLFW error code into a human-readable string and include the error description in the log message.
	 * 
	 * @param error The GLFW error code indicating the type of error that occurred.
	 * @param description A human-readable string describing the error that occurred.
	 */
	static void ErrorCallback(int error, const char* description);
private:
	/// @brief The application specification, which includes the application name, screen size, screen mode, and various settings.
	ApplicationSpecification m_Specification;

	/// @brief The handle to the main application window, which is a pointer to a `GLFWwindow` object.
	GLFWwindow* m_WindowHandle = nullptr;

	/// @brief A boolean flag indicating whether the application is currently running.
	bool m_Running = true;

	/// @brief A stack of layers that are currently attached to the application.
	std::vector<Layer*> m_LayerStack;

	/// @brief The time of the last frame, used for calculating the time elapsed between frames (delta time).
	float m_LastFrameTime = 0.0f;

	/// @brief The singleton instance of the application, which can be accessed globally using the `Get()` method.
	inline static Application* s_Instance = nullptr;

	/// @brief The application icon, loaded from a file and set as the window icon using GLFW.
	Scope<Image> m_AppIcon = nullptr;
};

/**
 * Factory function that creates and returns a pointer to a new instance of the `Application` class.
 * 
 * This function is defined in the platform-specific implementation file (e.g., `Platform/Windows/WindowsApplication.cpp`) and is responsible for creating the application instance with the appropriate platform-specific settings.
 * It allows the application to be created without exposing the implementation details of the `Application` class to the rest of the codebase, enabling a clean separation between the application logic and the platform-specific implementation.
 * 
 * @return A pointer to a new instance of the `Application` class.
 */
Application* CreateApplication();