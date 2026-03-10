#pragma once

#include <string>
#include <filesystem>

#include <glm/glm.hpp>

/**
 * Represents the different categories of settings available in the application.
 * 
 * Each category corresponds to a specific aspect of the application's configuration, allowing for organized management of settings.
 */
enum class Category {
	/// @brief General settings related to the application, such as name, version, and startup configuration.
	Application,

	/// @brief Settings specific to the editor interface, including appearance and auto-save options.
	Editor,

	/// @brief Settings related to graphics rendering, such as resolution and rendering engine.
	Graphics,

	/// @brief Settings for input devices and controls, including navigation speeds and scroll behaviour.
	Input,

	/// @brief Settings for language and regional preferences.
	Localization,

	/// @brief Settings that affect the quality of graphics and performance.
	Quality,

	/// @brief Settings specific to the rendering process, including engine selection and resolution.
	Rendering,

	/// @brief Settings related to time management within the application, such as frame rate and time.
	Time,

	/// @brief Settings for image and configuration export, including format, quality, and output folder.
	Export
};

/**
 * Represents the window display mode for the application.
 */
enum class WindowMode {
	/// @brief The application runs in a standard window with borders and title bar.
	Windowed,

	/// @brief The application runs in true fullscreen mode, taking over the display.
	Fullscreen,

	/// @brief The application runs in a borderless window that covers the entire screen.
	Borderless
};

/**
 * Represents the image format used when exporting frames.
 */
enum class ExportImageFormat {
	/// @brief Portable Network Graphics — lossless, best quality.
	PNG,

	/// @brief JPEG — lossy compression, smaller file sizes.
	JPEG,

	/// @brief Windows Bitmap — uncompressed, large files.
	BMP
};

/**
 * Represents the different rendering engines available for the application.
 * 
 * Each rendering engine corresponds to a specific graphics API that the application can utilize for rendering graphics.
 * The choice of rendering engine can affect performance, compatibility, and visual quality.
 */
enum class RenderingEngine {
	/// @brief A cross-platform graphics API that is widely supported and used for rendering 2D and 3D graphics.
	OpenGL,

	/// @brief A collection of APIs developed by Microsoft for handling tasks related to multimedia, especially game programming and video, primarily on Windows platforms.
	DirectX,

	/// @brief A low-overhead, cross-platform 3D graphics and compute API that provides high-efficiency access to modern GPUs, designed to offer better performance and more balanced CPU/GPU usage compared to older APIs like OpenGL and DirectX.
	Vulkan
};

/**
 * Represents the resolution settings for the application, including width, height, and scale.
 * 
 * The resolution settings determine the dimensions of the application's window and how the content is scaled within it.
 */
struct ResolutionSettings {
	/// @brief The width of the application window in pixels.
	int Width = 1920;

	/// @brief The height of the application window in pixels.
	int Height = 1080;

	/// @brief A scaling factor that can be applied to the content within the application window, allowing for adjustments to the size of UI elements and overall display without changing the actual resolution.
	float Scale = 1.0f;
};

/**
 * Represents the rendering settings for the application, including the rendering engine, resolution, window mode, and VSync.
 * 
 * The rendering settings determine how the application renders graphics and can affect performance and visual quality.
 */
struct RenderingSettings {
	/// @brief The rendering engine to be used for graphics rendering, which can be `OpenGL`, `DirectX`, or `Vulkan`.
	RenderingEngine Engine = RenderingEngine::OpenGL;

	/// @brief The resolution settings that specify the width, height, and scale of the application window.
	ResolutionSettings Resolution;

	/// @brief The window display mode: Windowed, Fullscreen, or Borderless.
	WindowMode Mode = WindowMode::Windowed;

	/// @brief A boolean value that indicates whether vertical synchronization (`VSync`) is enabled, which can help prevent screen tearing by synchronizing the frame rate of the application with the refresh rate of the monitor.
	bool VSync = true;

	/// @brief A boolean value that indicates whether to lock the frame rate to the target frame rate specified in `TargetFrameRate`.
	bool LockFramerate = false;

	/// @brief The target frame rate cap in frames per second.
	int TargetFrameRate = 0;
};

/**
 * Represents the version settings for the application, including major, minor, and patch version numbers.
 * 
 * The version settings provide information about the current version of the application, which can be useful for tracking updates, compatibility, and user support.
 */
struct VersionSettings {
	/// @brief The major version number, which typically indicates significant changes or milestones in the application's development.
	int Major = 1;

	/// @brief The minor version number, which usually indicates smaller updates, new features, or improvements that are added to the application without breaking compatibility.
	int Minor = 0;

	/// @brief The patch version number, which often indicates bug fixes, performance improvements, or minor changes that do not introduce new features or break compatibility.
	int Patch = 0;

	/**
	 * Gets the version as a string in the format `Major` `.` `Minor` `.` `Patch`.
	 * 
	 * @return A string representation of the version number.
	 */
	const std::string GetVersion() const {
		return (
			std::to_string(Major) + "." +
			std::to_string(Minor) + "." +
			std::to_string(Patch)
		);
	}

	/**
	 * Gets the version name as a string in the format `v` `Major` `.` `Minor` `.` `Patch`.
	 *
	 * @return A string representation of the version name.
	 */
	const std::string GetName() const {
		return "v" + GetVersion();
	}
};

/**
 * Represents the application settings for the application, including the name, startup configuration, version, and various options.
 * 
 * The application settings provide general configuration options for the application, such as its name, version, and behavior on startup.
 */
struct ApplicationSettings {
	/// @brief The name of the application, which can be displayed in the title bar and used for identification purposes.
	std::string Name = "Mandelbrot++";

	/// @brief The file path to the startup configuration, which specifies the initial settings or state of the application when it is launched.
	std::filesystem::path StartupConfiguration = "Internal/Configurations/Default.fractal";

	/// @brief The version settings that provide information about the current version of the application.
	VersionSettings Version;

	/// @brief A boolean value that indicates whether the application should start in a maximized state, filling the entire screen.
	bool Maximized = true;

	/// @brief A boolean value that indicates whether the application is running in debug mode, which can enable additional logging, error reporting, or debugging features for development purposes.
	bool DebugMode = false;

	/// @brief A boolean value that indicates whether pressing the Escape key should close the application, providing a quick way for users to exit the application using the keyboard.
	bool EscapeClosesApp = false;

	/// @brief A boolean value that indicates whether the application should write its log output to a file on disk.
	bool LogToFile = false;
};

/**
 * Represents the different editor themes available for the application.
 * 
 * Each editor theme corresponds to a specific visual style for the editor interface, allowing users to customize the appearance of the editor according to their preferences.
 */
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

/**
 * Represents the appearance settings for the editor, including the theme, font size, UI scale, and column width.
 * 
 * The appearance settings allow users to customize the visual aspects of the editor interface, such as the color scheme, font size, and layout, to enhance their user experience and productivity.
 */
struct AppearanceSettings {
	/// @brief The editor theme to be used for the editor interface, which can be one of the predefined themes.
	EditorTheme Theme = EditorTheme::Excellency;

	/// @brief The font size used in the editor interface, which can affect the readability and overall appearance of text elements within the editor.
	int FontSize = 14;

	/// @brief A scaling factor that can be applied to the user interface (UI) elements within the editor, allowing users to adjust the size of UI components for better visibility or to fit their screen resolution.
	float UIScale = 1.0f;

	/// @brief The width of columns in the editor interface, which can affect the layout and organization of content within the editor.
	float ColumnWidth = 200.0f;
};

/**
 * Represents the default visibility of editor windows on startup.
 */
struct WindowsSettings {
	/// @brief Whether the About window is visible on startup.
	bool ShowAbout = false;

	/// @brief Whether the Inspector window is visible on startup.
	bool ShowInspector = true;

	/// @brief Whether the Project window is visible on startup.
	bool ShowProject = false;

	/// @brief Whether the Settings window is visible on startup.
	bool ShowSettings = false;

	/// @brief Whether the Statistics window is visible on startup.
	bool ShowStatistics = false;

	/// @brief Whether the Viewport window is visible on startup.
	bool ShowViewport = true;
};

/**
 * Represents the editor settings for the application, including appearance settings and auto-save interval.
 * 
 * The editor settings provide configuration options specific to the editor interface, allowing users to customize the appearance and behavior of the editor according to their preferences.
 */
struct EditorSettings {
	/// @brief The appearance settings that allow users to customize the visual aspects of the editor interface, such as the theme, font size, UI scale, and column width.
	AppearanceSettings Appearance;

	/// @brief The default visibility of editor windows on startup.
	WindowsSettings Windows;

	/// @brief The interval in minutes for the auto-save feature, which determines how frequently the editor automatically saves the user's work to prevent data loss.
	int AutoSaveInterval = 10;
};

/**
 * Represents settings that control the interactive navigation of the fractal viewport.
 */
struct NavigationSettings {
	/// @brief Speed multiplier for panning/movement. Higher values move the view faster.
	float MovementSpeed = 2.0f;

	/// @brief Speed multiplier for rotation (Q/E keys). Higher values rotate faster.
	float RotationSpeed = 2.0f;

	/// @brief Speed multiplier for zooming (scroll wheel / Shift / Ctrl). Higher values zoom faster.
	float ZoomSpeed = 2.0f;

	/// @brief Speed multiplier for changing the fractal power (PageUp / PageDown keys).
	float PowerSpeed = 2.0f;

	/// @brief Smoothing factor for parameter interpolation. Higher values produce snappier, faster transitions.
	float Smoothing = 5.0f;

	/// @brief When enabled, the scroll wheel zoom direction is reversed.
	bool InvertZoom = false;
};

/**
 * Represents settings that control how fractal frames and configurations are exported.
 */
struct ExportSettings {
	/// @brief The image format used when exporting a frame (PNG, JPEG, or BMP).
	ExportImageFormat ImageFormat = ExportImageFormat::PNG;

	/// @brief JPEG compression quality in the range [0, 100]. Only relevant when `ImageFormat` is JPEG.
	int ImageQuality = 90;

	/// @brief Root folder where exported images and configurations are placed.
	std::filesystem::path Folder = "Export";
};

/**
 * Represents the overall settings for the application, including application settings, rendering settings, and editor settings.
 * 
 * The settings struct serves as a container for all the different categories of settings, allowing for organized management and easy access to various configuration options throughout the application.
 */
struct Settings {
	/// @brief The application settings that provide general configuration options for the application, such as its name, version, and behavior on startup.
	ApplicationSettings Application;

	/// @brief The rendering settings that determine how the application renders graphics, including the rendering engine, resolution, window mode, and VSync.
	RenderingSettings Rendering;

	/// @brief The editor settings that provide configuration options specific to the editor interface, allowing users to customize the appearance and behavior of the editor.
	EditorSettings Editor;

	/// @brief The navigation settings that control viewport interaction speeds and behaviour.
	NavigationSettings Navigation;

	/// @brief The export settings that control the format and destination of exported images and configurations.
	ExportSettings Export;
};