#pragma once

#include <string>
#include <filesystem>

#include <glm/glm.hpp>

enum class Category {
	Application,
	Editor,
	Graphics,
	Input,
	Localization,
	Quality,
	Rendering,
	Time
};

enum class RenderingEngine { OpenGL, DirectX, Vulkan };

struct ResolutionSettings {
	int Width = 1920;
	int Height = 1080;
	float Scale = 1.0f;
};

struct RenderingSettings {
	RenderingEngine Engine = RenderingEngine::OpenGL;
	ResolutionSettings Resolution;
	bool Fullscreen = false;
	bool VSync = true;
};

struct VersionSettings {
	int Major = 1;
	int Minor = 0;
	int Patch = 0;

	const std::string GetName() const {
		return "v" +
			std::to_string(Major) + "." +
			std::to_string(Minor) + "." +
			std::to_string(Patch);
	}
};

struct ApplicationSettings {
	std::string Name = "Mandelbrot++";
	std::filesystem::path StartupConfiguration = "Internal/Configurations/Default.fractal";

	VersionSettings Version;

	bool Maximized = true;
	bool DebugMode = false;
	bool EscapeClosesApp = false;
};

enum class EditorTheme {
	AdobeInspired,
	AyuDark,
	BessDark,
	BlackDevil,
	BootstrapDark,
	Carbon,
	Cherno,
	Cherry,
	ClassicSteam,
	Classic,
	ComfortableDarkCyan,
	ComfortableLightOrange,
	Comfy,
	Darcula,
	DarkRed,
	DarkRuda,
	Dark,
	Darky,
	DeepDark,
	DiscordDark,
	Enemymouse,
	Everforest,
	Excellency,
	FutureDark,
	Gold,
	GreenFont,
	GreenLeaf,
	HazyDark,
	LedSynthmaster,
	Light,
	MaterialFlat,
	Microsoft,
	Modern,
	Photoshop,
	PurpleComfy,
	QuickMinimalLook,
	RedFont,
	RedOni,
	Rest,
	RoundedVisualStudio,
	SoftCherry,
	SonicRiders,
	Unreal,
	VisualStudio,
	Windark
};

struct AppearanceSettings {
	EditorTheme Theme = EditorTheme::Excellency;
	int FontSize = 14;
	float UIScale = 1.0f;
	float ColumnWidth = 200.0f;
};

struct EditorSettings {
	AppearanceSettings Appearance;

	int AutoSaveInterval = 10;
};

struct Settings {
	ApplicationSettings Application;
	RenderingSettings Rendering;
	EditorSettings Editor;
};