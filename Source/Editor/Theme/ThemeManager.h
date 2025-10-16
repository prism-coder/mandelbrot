#pragma once

#include "Core/Settings/Settings.h"

#include "Editor/Theme/Theme.h"

#include "glm/glm.hpp"
#include "imgui.h"

#include <filesystem>

class ThemeManager {
public:
	static Theme& Get() { return s_Theme; }

	static const std::filesystem::path& GetCurrentThemePath() { return s_CurrentThemePath; }
	static const std::filesystem::path& GetThemesDirectory() { return s_ThemesPath; }

	static bool Load(const std::filesystem::path& filepath = s_ThemesPath);
	static bool Save(const std::filesystem::path& filepath = s_ThemesPath);

	static void Apply(const Theme& theme);

	static std::filesystem::path Select(const EditorTheme& theme);
private:
	static ImVec2 GetImVec2FromGlmVec2(const glm::vec2& vec);
	static ImVec4 GetImVec4FromGlmVec4(const glm::vec4& vec);
	static ImGuiDir GetImGuiDirFromDirection(const Direction& direction);
private:
	inline static std::filesystem::path s_ThemesPath = "Internal/Themes/";
	inline static std::filesystem::path s_CurrentThemePath;

	inline static Theme s_Theme;
};