#include "Application.h"

#include "Core/Log.h"

#include "Core/Input/Input.h"
#include "Core/Input/Cursor.h"

#include "Core/Settings/SettingsManager.h"

#include "Renderer/Renderer.h"

#include "Editor/UI.h"

#include <iostream>
#include <format>

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
	s_Instance = this;
	LogSpecification logSpecification;
	logSpecification.Name = spec.Name;
	logSpecification.Filepath = "Application.log";
	logSpecification.WriteToFile = spec.LogToFile;
	Log::Init(logSpecification);

	Init();
}

Application::~Application() {
	Shutdown();
}

void Application::Init() {
	Log::Trace("Application::Init - Initializing the Application");

	Log::Trace("Application::Init - Setting up GLFW Error Callback");
	glfwSetErrorCallback(ErrorCallback);

	if (!glfwInit()) {
		Log::Fatal("Application::Init - Failed to initialize GLFW");
		exit(EXIT_FAILURE);
	}

	Log::Trace("Application::Init - Initializing GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	Log::Trace("Application::Init - Initializing OpenGL");

	if (m_Specification.Maximized) {
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	}

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	Log::Trace("Application::Init - Initializing Monitor");

	if (m_Specification.ScreenMode == ScreenMode::Fullscreen) {
		m_Specification.ScreenSize.Width = mode->width;
		m_Specification.ScreenSize.Height = mode->height;
	} else if (m_Specification.ScreenMode == ScreenMode::Borderless) {
		m_Specification.ScreenSize.Width = mode->width;
		m_Specification.ScreenSize.Height = mode->height;
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	}

	Log::Trace("Application::Init - Creating Window");

	m_WindowHandle = glfwCreateWindow(
		m_Specification.ScreenSize.Width,
		m_Specification.ScreenSize.Height,
		m_Specification.Name.c_str(),
		m_Specification.ScreenMode == ScreenMode::Fullscreen ? monitor : NULL,
		NULL
	);

	if (!m_WindowHandle) {
		Log::Fatal("Application::Init - Failed to create Window");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_WindowHandle);
	glfwSetWindowUserPointer(m_WindowHandle, this);

	Log::Trace("Application::Init - Setting up Window callbacks");
	glfwSetKeyCallback(m_WindowHandle, KeyCallback);
	glfwSetMouseButtonCallback(m_WindowHandle, MouseButtonCallback);
	glfwSetCursorPosCallback(m_WindowHandle, CursorPosCallback);

	Log::Trace("Application::Init - Showing the Window");
	glfwShowWindow(m_WindowHandle);

	Log::Trace("Application::Init - Setting up the App Icon");
	LoadAppIcon();

	Log::Trace("Application::Init - Setting up the Window Title");
	SetWindowTitle("No Configuration");

	SetVSync(m_Specification.VSync);

	Renderer::Init();
	Cursor::Init(m_WindowHandle);
	UI::Init();
}

void Application::Shutdown() {
	Log::Trace("Application::Shutdown - Shutting down the Application");

	Log::Trace("Application::Shutdown - Detaching all Windows");
	for (Layer* layer : m_LayerStack) {
		layer->OnDetach();
		delete layer;
	}

	Log::Trace("Application::Shutdown - Clearing the Layer Stack");
	m_LayerStack.clear();

	Log::Trace("Application::Shutdown - Shutting down the Renderer");
	Renderer::Shutdown();

	Log::Trace("Application::Shutdown - Shutting down the Cursor");
	Cursor::Shutdown();

	Log::Trace("Application::Shutdown - Shutting down the UI");
	UI::Shutdown();

	Log::Trace("Application::Shutdown - Destroying Window");
	glfwDestroyWindow(m_WindowHandle);

	Log::Trace("Application::Shutdown - Terminating");
	glfwTerminate();
}

void Application::LoadAppIcon() {
	Log::Trace("Application::LoadAppIcon - Loading App Icon");

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
	while (!glfwWindowShouldClose(m_WindowHandle) && m_Running) {
		Input::OnUpdate();

		float time = (float)glfwGetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		UI::Begin();

		for (Layer* layer : m_LayerStack) {
			layer->OnUpdate(timestep);
			layer->OnUIRender();
		}

		UI::End();

		glfwSwapBuffers(m_WindowHandle);
		glfwPollEvents();
	}
}

void Application::PushLayer(Layer* layer) {
	m_LayerStack.push_back(layer);
	layer->OnAttach();
}

void Application::Close() {
	Log::Trace("Application::Close - Closing the Application");
	m_Running = false;
}

// --- Callbacks ---
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

void Application::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	Input::SetMousePosition(xpos, ypos);
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
