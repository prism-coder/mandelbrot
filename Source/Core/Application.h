#pragma once

#include "Core/Core.h"
#include "Core/Layer.h"
#include "Core/Image.h"

#include "Layers/Layers.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
#include <vector>

struct ScreenSize {
	int Width;
	int Height;
};

enum class ScreenMode {
	Windowed,
	Fullscreen,
	Borderless
};

struct ApplicationSpecification {
	std::string Name = "App";

	ScreenSize ScreenSize = { 1920, 1080 };
	ScreenMode ScreenMode = ScreenMode::Windowed;

	bool EscapeClosesApp = false;
	bool Maximized = false;
	bool VSync = true;
	bool LogToFile = true;
};

class Application {
public:
	Application(const ApplicationSpecification& spec = {});
	~Application();

	void Run();
	void PushLayer(Layer* layer);
	void Close();

	static Application& Get() { return *s_Instance; }
	GLFWwindow* GetWindowHandle() const { return m_WindowHandle; }

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;

	void SetVSync(bool enabled);
	void SetWindowTitle(const std::string& title);

	const ApplicationSpecification& GetSpecification() const { return m_Specification; }
private:
	void Init();
	void Shutdown();

	void LoadAppIcon();

	// Callbacks
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void ErrorCallback(int error, const char* description);
private:
	ApplicationSpecification m_Specification;
	GLFWwindow* m_WindowHandle = nullptr;
	bool m_Running = true;

	std::vector<Layer*> m_LayerStack;
	float m_LastFrameTime = 0.0f;

	inline static Application* s_Instance = nullptr;

	Scope<Image> m_AppIcon = nullptr;
};

// To be defined in CLIENT
Application* CreateApplication();