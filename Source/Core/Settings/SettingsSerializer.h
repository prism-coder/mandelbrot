#pragma once

#include "Core/Settings/Settings.h"

#include "yaml-cpp/yaml.h"

#include <filesystem>

class SettingsSerializer {
public:
	SettingsSerializer(Settings& settings);

	bool Serialize(const std::filesystem::path& filepath);
	bool Deserialize(const std::filesystem::path& filepath);
private:
	void SerializeApplicationSettings(YAML::Emitter& out);
	void SerializeEditorSettings(YAML::Emitter& out);
	void SerializeRenderingSettings(YAML::Emitter& out);

	void DeserializeApplicationSettings(const YAML::Node& settingsNode);
	void DeserializeEditorSettings(const YAML::Node& settingsNode);
	void DeserializeRenderingSettings(const YAML::Node& settingsNode);
private:
	Settings& m_Settings;
};