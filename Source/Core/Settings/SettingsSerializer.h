#pragma once

#include "Core/Settings/Settings.h"

#include "yaml-cpp/yaml.h"

#include <filesystem>

/**
 * Handles the serialization and deserialization of application settings to and from a YAML file.
 * 
 * The `SettingsSerializer` class provides methods to serialize the current application settings into a YAML format and save it to a file, as well as to read settings from a YAML file and deserialize them into the application's settings structure.
 * This allows for easy persistence of user preferences and configurations across sessions, enabling users to customize their experience and maintain their settings even after closing the application.
 */
class SettingsSerializer {
public:
	/**
	 * Constructs a `SettingsSerializer` instance with a reference to the settings that will be serialized or deserialized.
	 * 
	 * @param settings A reference to the `Settings` object that will be used for serialization and deserialization. This allows the serializer to read from and write to the settings as needed.
	 */
	SettingsSerializer(Settings& settings);

	/**
	 * Serializes the current settings to a YAML file at the specified file path.
	 * 
	 * @param filepath The path to the YAML file where the settings will be saved.
	 * @return `true` if the settings were successfully serialized and saved to the file, `false` otherwise.
	 */
	bool Serialize(const std::filesystem::path& filepath);

	/**
	 * Deserializes settings from a YAML file at the specified file path and updates the current settings accordingly.
	 * 
	 * @param filepath The path to the YAML file from which the settings will be loaded.
	 * @return `true` if the settings were successfully deserialized and loaded from the file, `false` otherwise.
	 */
	bool Deserialize(const std::filesystem::path& filepath);

private:

#pragma region - Serialization -
	/**
	 * Serializes the application settings to a YAML emitter.
	 * 
	 * @param out The YAML emitter to which the application settings will be serialized.
	 */
	void SerializeApplicationSettings(YAML::Emitter& out);

	/**
	 * Serializes the editor settings to a YAML emitter.
	 * 
	 * @param out The YAML emitter to which the editor settings will be serialized.
	 */
	void SerializeEditorSettings(YAML::Emitter& out);

	/**
	 * Serializes the rendering settings to a YAML emitter.
	 * 
	 * @param out The YAML emitter to which the rendering settings will be serialized.
	 */
	void SerializeRenderingSettings(YAML::Emitter& out);

	/**
	 * Serializes the navigation settings to a YAML emitter.
	 * 
	 * @param out The YAML emitter to which the navigation settings will be serialized.
	 */
	void SerializeNavigationSettings(YAML::Emitter& out);

	/**
	 * Serializes the export settings to a YAML emitter.
	 * 
	 * @param out The YAML emitter to which the export settings will be serialized.
	 */
	void SerializeExportSettings(YAML::Emitter& out);
#pragma endregion

#pragma region - Deserialization -
	/**
	 * Deserializes the application settings from a YAML node and updates the current settings accordingly.
	 * 
	 * @param settingsNode The YAML node containing the application settings to be deserialized.
	 */
	void DeserializeApplicationSettings(const YAML::Node& settingsNode);

	/**
	 * Deserializes the editor settings from a YAML node and updates the current settings accordingly.
	 * 
	 * @param settingsNode The YAML node containing the editor settings to be deserialized.
	 */
	void DeserializeEditorSettings(const YAML::Node& settingsNode);

	/**
	 * Deserializes the rendering settings from a YAML node and updates the current settings accordingly.
	 * 
	 * @param settingsNode The YAML node containing the rendering settings to be deserialized.
	 */
	void DeserializeRenderingSettings(const YAML::Node& settingsNode);

	/**
	 * Deserializes the navigation settings from a YAML node and updates the current settings accordingly.
	 * 
	 * @param settingsNode The YAML node containing the navigation settings to be deserialized.
	 */
	void DeserializeNavigationSettings(const YAML::Node& settingsNode);

	/**
	 * Deserializes the export settings from a YAML node and updates the current settings accordingly.
	 * 
	 * @param settingsNode The YAML node containing the export settings to be deserialized.
	 */
	void DeserializeExportSettings(const YAML::Node& settingsNode);
#pragma endregion

private:
	/// @brief A reference to the `Settings` object that will be used for serialization and deserialization.
	Settings& m_Settings;
};