#pragma once

#include "Editor/Theme/Theme.h"

#include "yaml-cpp/yaml.h"

#include <filesystem>

class ThemeSerializer {
public:
	ThemeSerializer(Theme& theme);

	bool Serialize(const std::filesystem::path& filepath);
	bool Deserialize(const std::filesystem::path& filepath);
private:
	void SerializeStyle(YAML::Emitter& out);
	void SerializeColors(YAML::Emitter& out);

	void DeserializeStyle(const YAML::Node& themeNode);
	void DeserializeColors(const YAML::Node& themeNode);
private:
	Theme& m_Theme;
};