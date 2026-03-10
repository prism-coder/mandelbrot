#include "Core/Application.h"
#include "Core/EntryPoint.h"

#include "Core/Settings/SettingsManager.h"

#include "Layers/Layers.h"

Application* CreateApplication() {
	SettingsManager::Load();

	Settings& settings = SettingsManager::Get();

	ApplicationSpecification spec;
	spec.Name = settings.Application.Name;
	spec.ScreenSize = {
		settings.Rendering.Resolution.Width,
		settings.Rendering.Resolution.Height
	};

	switch (settings.Rendering.Mode) {
		case WindowMode::Fullscreen:	spec.ScreenMode = ScreenMode::Fullscreen;	break;
		case WindowMode::Borderless:	spec.ScreenMode = ScreenMode::Borderless;	break;
		default:						spec.ScreenMode = ScreenMode::Windowed;		break;
	}

	spec.Maximized = settings.Application.Maximized;
	spec.EscapeClosesApp = settings.Application.EscapeClosesApp;
	spec.VSync = settings.Rendering.VSync;
	spec.LogToFile = settings.Application.LogToFile;
	spec.LockFramerate = settings.Rendering.LockFramerate;
	spec.TargetFrameRate = settings.Rendering.TargetFrameRate;

	Application* app = new Application(spec);
	app->PushLayer(new MandelbrotLayer());

	return app;
}