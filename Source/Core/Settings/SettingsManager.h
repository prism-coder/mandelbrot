#pragma once

#include "Core/Settings/Settings.h"

#include <filesystem>

class SettingsManager {
public:
	static Settings& Get() { return s_Settings; }

	static bool Load(const std::filesystem::path& filepath = s_SettingsPath);
	static bool Save(const std::filesystem::path& filepath = s_SettingsPath);
private:
	inline static std::filesystem::path s_SettingsPath = "Settings.yaml";
	inline static Settings s_Settings;
};