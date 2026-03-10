#include "Application.h"

#include "Core/Log.h"

#include "Core/Input/Input.h"
#include "Core/Input/Cursor.h"

#include "Core/Settings/SettingsManager.h"

#include "Renderer/Renderer.h"

#include "Editor/UI.h"

#include <iostream>
#include <format>
#include <thread>
#include <chrono>

/**
 * Converts a GLFW error code into a human-readable string representation.
 * 
 * @param error The GLFW error code to convert.
 * @return A string representation of the GLFW error code.
 */
std::string GLFWErrorToString(int error) {
	switch (error) {
		case GLFW_NO_ERROR:					return "GLFW_NO_ERROR";
		case GLFW_NOT_INITIALIZED:			return "GLFW_NOT_INITIALIZED";
		case GLFW_NO_CURRENT_CONTEXT:		return "GLFW_NO_CURRENT_CONTEXT";
		case GLFW_INVALID_ENUM:				return "GLFW_INVALID_ENUM";
		case GLFW_INVALID_VALUE:			return "GLFW_INVALID_VALUE";
		case GLFW_OUT_OF_MEMORY:			return "GLFW_OUT_OF_MEMORY";
		case GLFW_API_UNAVAILABLE:			return "GLFW_API_UNAVAILABLE";
		case GLFW_VERSION_UNAVAILABLE:		return "GLFW_VERSION_UNAVAILABLE";
		case GLFW_PLATFORM_ERROR:			return "GLFW_PLATFORM_ERROR";
		case GLFW_FORMAT_UNAVAILABLE:		return "GLFW_FORMAT_UNAVAILABLE";
		case GLFW_NO_WINDOW_CONTEXT:		return "GLFW_NO_WINDOW_CONTEXT";
		case GLFW_CURSOR_UNAVAILABLE:		return "GLFW_CURSOR_UNAVAILABLE";
		case GLFW_FEATURE_UNAVAILABLE:		return "GLFW_FEATURE_UNAVAILABLE";
		case GLFW_FEATURE_UNIMPLEMENTED:	return "GLFW_FEATURE_UNIMPLEMENTED";
		case GLFW_PLATFORM_UNAVAILABLE:		return "GLFW_PLATFORM_UNAVAILABLE";
		default:							return "UNKNOWN";
	}
}

Application::Application(const ApplicationSpecification& spec)
	: m_Specification(spec)
{
	// Set the static instance pointer to this instance of the application.
	s_Instance = this;

	// Create a log specification based on the application specification.
	LogSpecification logSpecification;
	logSpecification.Name = spec.Name;
	logSpecification.Filepath = "Application.log";
	logSpecification.WriteToFile = spec.LogToFile;

	// Initialize the logging system with the specified log specification.
	Log::Init(logSpecification);

	// Initialize the application, including setting up GLFW, creating the window, and loading resources.
	Init();
}

Application::~Application() {
	// Shut down the application, cleaning up resources and terminating GLFW.
	Shutdown();
}

void Application::Init() {
	Log::Trace("Application::Init - Initializing the Application");

	Log::Trace("Application::Init - Setting up GLFW Error Callback");
	glfwSetErrorCallback(ErrorCallback);

	// Initialize GLFW and check for errors.
	if (!glfwInit()) {
		Log::Fatal("Application::Init - Failed to initialize GLFW");

		// Exit the application with a failure status code if GLFW initialization fails.
		exit(EXIT_FAILURE);
	}

	Log::Trace("Application::Init - Initializing GLFW");

	// Set GLFW window hints for the OpenGL context version, profile, and debug context.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	Log::Trace("Application::Init - Initializing OpenGL");

	if (m_Specification.Maximized) {
		// Set the GLFW window hint to start the window maximized if the specification indicates it should be.
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	}

	// Get the primary monitor and its video mode to set up the window with the appropriate settings based on the screen mode specified in the application specification.
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	// Set GLFW window hints based on the video mode of the primary monitor, including color bits and refresh rate.
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	Log::Trace("Application::Init - Initializing Monitor");

	// Adjust the screen size in the application specification based on the selected screen mode (fullscreen or borderless).
	if (m_Specification.ScreenMode == ScreenMode::Fullscreen) {
		m_Specification.ScreenSize.Width = mode->width;
		m_Specification.ScreenSize.Height = mode->height;
	} else if (m_Specification.ScreenMode == ScreenMode::Borderless) {
		m_Specification.ScreenSize.Width = mode->width;
		m_Specification.ScreenSize.Height = mode->height;
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	}

	Log::Trace("Application::Init - Creating Window");

	// Create the GLFW window with the specified width, height, title, and monitor settings based on the screen mode.
	m_WindowHandle = glfwCreateWindow(
		m_Specification.ScreenSize.Width,
		m_Specification.ScreenSize.Height,
		m_Specification.Name.c_str(),
		m_Specification.ScreenMode == ScreenMode::Fullscreen ? monitor : NULL,
		NULL
	);

	// Check if the window was created successfully and handle errors if it was not.
	if (!m_WindowHandle) {
		Log::Fatal("Application::Init - Failed to create Window");

		// Terminate GLFW if window creation fails.
		glfwTerminate();

		// Exit the application with a failure status code if window creation fails.
		exit(EXIT_FAILURE);
	}

	// Make the OpenGL context of the window current and set the user pointer for the window to this instance of the application.
	glfwMakeContextCurrent(m_WindowHandle);
	glfwSetWindowUserPointer(m_WindowHandle, this);

	Log::Trace("Application::Init - Setting up Window callbacks");

	// Set GLFW callbacks for key input, mouse button input, mouse scroll, and cursor position to the corresponding static callback functions defined in the `Application` class.
	glfwSetKeyCallback(m_WindowHandle, KeyCallback);
	glfwSetMouseButtonCallback(m_WindowHandle, MouseButtonCallback);
	glfwSetScrollCallback(m_WindowHandle, MouseScrollCallback);
	glfwSetCursorPosCallback(m_WindowHandle, CursorPosCallback);

	// Show the window after it has been created and configured.
	Log::Trace("Application::Init - Showing the Window");
	glfwShowWindow(m_WindowHandle);

	// Load the application icon from a file and set it as the window icon using GLFW.
	Log::Trace("Application::Init - Setting up the App Icon");
	LoadAppIcon();

	// Set the initial window title, which includes the application name and version information.
	Log::Trace("Application::Init - Setting up the Window Title");
	SetWindowTitle("No Configuration");

	// Set VSync based on the application specification.
	SetVSync(m_Specification.VSync);

	// Initialize the renderer, which will set up the OpenGL context and prepare for rendering.
	Log::Trace("Application::Init - Initializing the Renderer");
	Renderer::Init();

	// Initialize the cursor management system with the GLFW window handle.
	Log::Trace("Application::Init - Initializing the Cursor");
	Cursor::Init(m_WindowHandle);

	// Initialize the UI system, which will set up ImGui and prepare for rendering the user interface.
	Log::Trace("Application::Init - Initializing the UI");
	UI::Init();
}

void Application::Shutdown() {
	Log::Trace("Application::Shutdown - Shutting down the Application");

	// Detach and delete all layers in the layer stack to clean up resources before shutting down the application.
	Log::Trace("Application::Shutdown - Detaching all Windows");
	for (Layer* layer : m_LayerStack) {
		layer->OnDetach();
		delete layer;
	}

	// Clear the layer stack after detaching and deleting all layers to free up resources.
	Log::Trace("Application::Shutdown - Clearing the Layer Stack");
	m_LayerStack.clear();

	// Shut down the renderer, which will clean up OpenGL resources and prepare for application exit.
	Log::Trace("Application::Shutdown - Shutting down the Renderer");
	Renderer::Shutdown();

	// Shut down the cursor management system, which will release any resources and reset the state.
	Log::Trace("Application::Shutdown - Shutting down the Cursor");
	Cursor::Shutdown();

	// Shut down the UI system, which will clean up ImGui resources and prepare for application exit.
	Log::Trace("Application::Shutdown - Shutting down the UI");
	UI::Shutdown();

	// Destroy the GLFW window and terminate GLFW to clean up resources before exiting the application.
	Log::Trace("Application::Shutdown - Destroying Window");
	glfwDestroyWindow(m_WindowHandle);

	// Terminate GLFW to clean up any remaining resources and prepare for application exit.
	Log::Trace("Application::Shutdown - Terminating");
	glfwTerminate();
}

void Application::LoadAppIcon() {
	Log::Trace("Application::LoadAppIcon - Loading App Icon");

	// Load the application icon from a file and check if it was loaded successfully.
	m_AppIcon = CreateScope<Image>("Internal/Icons/AppIcon/appicon.png");
	if (m_AppIcon->IsLoaded()) {
		GLFWimage images[1];
		images[0].width = m_AppIcon->GetWidth();
		images[0].height = m_AppIcon->GetHeight();
		images[0].pixels = (unsigned char*)m_AppIcon->GetPixels();

		glfwSetWindowIcon(m_WindowHandle, 1, images);
	}
}

void Application::Run() {
	Log::Trace("Application::Run - Starting the main loop");

	// Main application loop that continues until the window should close or the application is signaled to stop running.
	while (!glfwWindowShouldClose(m_WindowHandle) && m_Running) {
		// Record the frame start time for frame-rate limiting.
		double frameStart = glfwGetTime();

		// Calculate the time elapsed since the last frame to use for updating layers and managing timing.
		float time = (float)frameStart;
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		// Begin the UI frame, which will prepare for rendering the user interface for this frame.
		UI::Begin();

		for (Layer* layer : m_LayerStack) {
			// Update each layer with the calculated timestep.
			layer->OnUpdate(timestep);

			// Render the UI for each layer.
			layer->OnUIRender();
		}

		// End the UI frame, which will finalize the rendering of the user interface for this frame.
		UI::End();

		// Update the input state at the end of the frame to ensure that input states are correctly updated for the next frame.
		Input::OnUpdate();

		// Swap the front and back buffers to display the rendered frame and poll for input events to process user interactions.
		glfwSwapBuffers(m_WindowHandle);

		// Poll for and process input events, which will trigger the appropriate callbacks for key input, mouse input, and other events.
		glfwPollEvents();

		// Apply frame-rate cap if a target is set (TargetFrameRate > 0) and frame rate locking is enabled.
		if (m_Specification.LockFramerate && m_Specification.TargetFrameRate > 0) {
			double targetDt = 1.0 / static_cast<double>(m_Specification.TargetFrameRate);
			double elapsed = glfwGetTime() - frameStart;
			
			if (elapsed < targetDt) {
				std::this_thread::sleep_for(std::chrono::duration<double>(targetDt - elapsed));
			}
		}
	}

	Log::Trace("Application::Run - Stopping the main loop");
}

void Application::PushLayer(Layer* layer) {
	m_LayerStack.push_back(layer);
	layer->OnAttach();
}

void Application::Close() {
	Log::Trace("Application::Close - Closing the Application");
	m_Running = false;
}

void Application::ErrorCallback(int error, const char* description) {
	Log::Error("GLFW Error (" + GLFWErrorToString(error) + "): " + description);
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	KeyCode engineKey = static_cast<KeyCode>(key);
	InputAction engineAction = InputAction::Unknown;

	if (action == GLFW_PRESS) {
		engineAction = InputAction::Press;
	} else if (action == GLFW_RELEASE) {
		engineAction = InputAction::Release;
	} else if (action == GLFW_REPEAT) {
		engineAction = InputAction::Repeat;
	}

	Input::SetKeyState(engineKey, engineAction);
}

void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	MouseCode engineButton = static_cast<MouseCode>(button);
	InputAction engineAction = InputAction::Unknown;

	if (action == GLFW_PRESS) {
		engineAction = InputAction::Press;
	} else if (action == GLFW_RELEASE) {
		engineAction = InputAction::Release;
	} else if (action == GLFW_REPEAT) {
		engineAction = InputAction::Repeat;
	}

	Input::SetMouseButtonState(engineButton, engineAction);
}

void Application::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Input::SetScrollOffset(xoffset, yoffset);
}

void Application::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	Input::SetMousePosition(xpos, ypos);
}

void Application::GamepadButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	GamepadCode engineButton = static_cast<GamepadCode>(button);
	InputAction engineAction = InputAction::Unknown;

	if (action == GLFW_PRESS) {
		engineAction = InputAction::Press;
	} else if (action == GLFW_RELEASE) {
		engineAction = InputAction::Release;
	} else if (action == GLFW_REPEAT) {
		engineAction = InputAction::Repeat;
	}

	Input::SetGamepadButtonState(engineButton, engineAction);
}

void Application::GamepadAxisCallback(GLFWwindow* window, int axis, float value) {

}

uint32_t Application::GetWidth() const {
	int width, height;
	glfwGetFramebufferSize(m_WindowHandle, &width, &height);
	return static_cast<uint32_t>(width);
}

uint32_t Application::GetHeight() const {
	int width, height;
	glfwGetFramebufferSize(m_WindowHandle, &width, &height);
	return static_cast<uint32_t>(height);
}

void Application::SetVSync(bool enabled) {
	glfwSwapInterval(enabled ? 1 : 0);
}

void Application::SetWindowTitle(const std::string& title) {
	const auto& version = SettingsManager::Get().Application.Version;

	const auto& appName = m_Specification.Name;
	const auto& versionName = version.GetName();

	std::string titleWithVersion = appName + " - " + title + " - " + versionName;
	glfwSetWindowTitle(m_WindowHandle, titleWithVersion.c_str());
}
