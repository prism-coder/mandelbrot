#pragma once

#include "Core/Settings/Settings.h"

#include <filesystem>

/**
 * Manages the application settings, providing functionality to load and save settings from a file.
 * 
 * The `SettingsManager` class is responsible for handling the application's configuration, allowing users to persist their preferences and configurations across sessions.
 * It provides static methods to access the current settings and to load/save settings from/to a specified file path.
 */
class SettingsManager {
public:
	/**
	 * Retrieves the current application settings.
	 * 
	 * @return A reference to the current settings.
	 */
	static Settings& Get() { return s_Settings; }

	/**
	 * Loads the application settings from a specified file.
	 *
	 * @param filepath The path to the settings file.
	 * @return `true` if the settings were successfully loaded, `false` otherwise.
	 */
	static bool Load(const std::filesystem::path& filepath = s_SettingsPath);

	/**
	 * Saves the current application settings to a specified file.
	 * 
	 * @param filepath The path to the settings file.
	 * @return `true` if the settings were successfully saved, `false` otherwise.
	 */
	static bool Save(const std::filesystem::path& filepath = s_SettingsPath);
private:
	/// @brief The default file path for the settings file, which is used when loading or saving settings if no specific path is provided.
	inline static std::filesystem::path s_SettingsPath = "Settings.yaml";

	/// @brief The current application settings, which are stored in memory and can be accessed or modified by the application.
	inline static Settings s_Settings;
};