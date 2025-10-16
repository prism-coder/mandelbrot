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
	spec.ScreenMode = settings.Rendering.Fullscreen ? ScreenMode::Fullscreen : ScreenMode::Windowed;
	spec.Maximized = settings.Application.Maximized;
	spec.LogToFile = settings.Application.DebugMode;

	Application* app = new Application(spec);
	app->PushLayer(new MandelbrotLayer());

	return app;
}