#include "ThemeManager.h"

#include "Core/Log.h"

#include "Editor/Theme/ThemeSerializer.h"

bool ThemeManager::Load(const std::filesystem::path& filepath) {
	Log::Info("ThemeManager::Load - Loading Theme: " + filepath.string());

	ThemeSerializer serializer(s_Theme);

	if (serializer.Deserialize(filepath)) {
		Log::Info("ThemeManager::Load - Theme loaded: " + filepath.string());

		s_CurrentThemePath = filepath;

		return true;
	}

	Log::Error("ThemeManager::Load - Couldn't load Theme: " + filepath.string());

	return false;
}

bool ThemeManager::Save(const std::filesystem::path& filepath) {
	Log::Info("ThemeManager::Save - Saving Theme: " + filepath.string());

	ThemeSerializer serializer(s_Theme);

	if (serializer.Serialize(filepath)) {
		Log::Info("ThemeManager::Save - Theme saved: " + filepath.string());

		return true;
	}

	Log::Error("ThemeManager::Save - Couldn't save Theme: " + filepath.string());

	return false;
}

std::filesystem::path ThemeManager::Select(const EditorTheme& theme) {
	Log::Trace("ThemeManager::Select - Selecting Theme");

	switch (theme) {
		case EditorTheme::AdobeInspired:			return "Internal/Themes/Adobe Inspired.yaml";
		case EditorTheme::AyuDark:					return "Internal/Themes/Ayu Dark.yaml";
		case EditorTheme::BessDark:					return "Internal/Themes/Bess Dark.yaml";
		case EditorTheme::BlackDevil:				return "Internal/Themes/Black Devil.yaml";
		case EditorTheme::BootstrapDark:			return "Internal/Themes/Bootstrap Dark.yaml";
		case EditorTheme::Carbon:					return "Internal/Themes/Carbon.yaml";
		case EditorTheme::Cherno:					return "Internal/Themes/Cherno.yaml";
		case EditorTheme::Cherry:					return "Internal/Themes/Cherry.yaml";
		case EditorTheme::ClassicSteam:				return "Internal/Themes/Classic Steam.yaml";
		case EditorTheme::Classic:					return "Internal/Themes/Classic.yaml";
		case EditorTheme::ComfortableDarkCyan:		return "Internal/Themes/Comfortable Dark Cyan.yaml";
		case EditorTheme::ComfortableLightOrange:	return "Internal/Themes/Comfortable Light Orange.yaml";
		case EditorTheme::Comfy:					return "Internal/Themes/Comfy.yaml";
		case EditorTheme::Darcula:					return "Internal/Themes/Darcula.yaml";
		case EditorTheme::DarkRed:					return "Internal/Themes/Dark Red.yaml";
		case EditorTheme::DarkRuda:					return "Internal/Themes/Dark Ruda.yaml";
		case EditorTheme::Dark:						return "Internal/Themes/Dark.yaml";
		case EditorTheme::Darky:					return "Internal/Themes/Darky.yaml";
		case EditorTheme::DeepDark:					return "Internal/Themes/Deep Dark.yaml";
		case EditorTheme::DiscordDark:				return "Internal/Themes/Discord Dark.yaml";
		case EditorTheme::Enemymouse:				return "Internal/Themes/Enemymouse.yaml";
		case EditorTheme::Everforest:				return "Internal/Themes/Everforest.yaml";
		case EditorTheme::Excellency:				return "Internal/Themes/Excellency.yaml";
		case EditorTheme::FutureDark:				return "Internal/Themes/Future Dark.yaml";
		case EditorTheme::Gold:						return "Internal/Themes/Gold.yaml";
		case EditorTheme::GreenFont:				return "Internal/Themes/Green Font.yaml";
		case EditorTheme::GreenLeaf:				return "Internal/Themes/Green Leaf.yaml";
		case EditorTheme::HazyDark:					return "Internal/Themes/Hazy Dark.yaml";
		case EditorTheme::LedSynthmaster:			return "Internal/Themes/Led Synthmaster.yaml";
		case EditorTheme::Light:					return "Internal/Themes/Light.yaml";
		case EditorTheme::MaterialFlat:				return "Internal/Themes/Material Flat.yaml";
		case EditorTheme::Microsoft:				return "Internal/Themes/Microsoft.yaml";
		case EditorTheme::Modern:					return "Internal/Themes/Modern.yaml";
		case EditorTheme::Photoshop:				return "Internal/Themes/Photoshop.yaml";
		case EditorTheme::PurpleComfy:				return "Internal/Themes/Purple Comfy.yaml";
		case EditorTheme::QuickMinimalLook:			return "Internal/Themes/Quick Minimal Look.yaml";
		case EditorTheme::RedFont:					return "Internal/Themes/Red Font.yaml";
		case EditorTheme::RedOni:					return "Internal/Themes/Red Oni.yaml";
		case EditorTheme::Rest:						return "Internal/Themes/Rest.yaml";
		case EditorTheme::RoundedVisualStudio:		return "Internal/Themes/Rounded Visual Studio.yaml";
		case EditorTheme::SoftCherry:				return "Internal/Themes/Soft Cherry.yaml";
		case EditorTheme::SonicRiders:				return "Internal/Themes/Sonic Riders.yaml";
		case EditorTheme::Unreal:					return "Internal/Themes/Unreal.yaml";
		case EditorTheme::VisualStudio:				return "Internal/Themes/Visual Studio.yaml";
		case EditorTheme::Windark:					return "Internal/Themes/Windark.yaml";
		default:									return "Internal/Themes/Excellency.yaml";
	}
}

void ThemeManager::Apply(const Theme& theme) {
	Log::Trace("ThemeManager::Apply - Applying Theme");

	ImGuiStyle& style = ImGui::GetStyle();

	// Style
	style.Alpha = theme.Style.Alpha;
	style.DisabledAlpha = theme.Style.DisabledAlpha;
	style.WindowPadding = GetImVec2FromGlmVec2(theme.Style.WindowPadding);
	style.WindowRounding = theme.Style.WindowRounding;
	style.WindowBorderSize = theme.Style.WindowBorderSize;
	style.WindowMinSize = GetImVec2FromGlmVec2(theme.Style.WindowMinSize);
	style.WindowTitleAlign = GetImVec2FromGlmVec2(theme.Style.WindowTitleAlign);
	style.WindowMenuButtonPosition = GetImGuiDirFromDirection(theme.Style.WindowMenuButtonPosition);
	style.ChildRounding = theme.Style.ChildRounding;
	style.ChildBorderSize = theme.Style.ChildBorderSize;
	style.PopupRounding = theme.Style.PopupRounding;
	style.PopupBorderSize = theme.Style.PopupBorderSize;
	style.FramePadding = GetImVec2FromGlmVec2(theme.Style.FramePadding);
	style.FrameRounding = theme.Style.FrameRounding;
	style.FrameBorderSize = theme.Style.FrameBorderSize;
	style.ItemSpacing = GetImVec2FromGlmVec2(theme.Style.ItemSpacing);
	style.ItemInnerSpacing = GetImVec2FromGlmVec2(theme.Style.ItemInnerSpacing);
	style.CellPadding = GetImVec2FromGlmVec2(theme.Style.CellPadding);
	style.IndentSpacing = theme.Style.IndentSpacing;
	style.ColumnsMinSpacing = theme.Style.ColumnsMinSpacing;
	style.ScrollbarSize = theme.Style.ScrollbarSize;
	style.ScrollbarRounding = theme.Style.ScrollbarRounding;
	style.GrabMinSize = theme.Style.GrabMinSize;
	style.GrabRounding = theme.Style.GrabRounding;
	style.TabRounding = theme.Style.TabRounding;
	style.TabBorderSize = theme.Style.TabBorderSize;
	style.ColorButtonPosition = GetImGuiDirFromDirection(theme.Style.ColorButtonPosition);
	style.ButtonTextAlign = GetImVec2FromGlmVec2(theme.Style.ButtonTextAlign);
	style.SelectableTextAlign = GetImVec2FromGlmVec2(theme.Style.SelectableTextAlign);

	// Colors
	style.Colors[ImGuiCol_Text] = GetImVec4FromGlmVec4(theme.Colors.Text);
	style.Colors[ImGuiCol_Text] = GetImVec4FromGlmVec4(theme.Colors.Text);
	style.Colors[ImGuiCol_TextDisabled] = GetImVec4FromGlmVec4(theme.Colors.TextDisabled);
	style.Colors[ImGuiCol_WindowBg] = GetImVec4FromGlmVec4(theme.Colors.WindowBg);
	style.Colors[ImGuiCol_ChildBg] = GetImVec4FromGlmVec4(theme.Colors.ChildBg);
	style.Colors[ImGuiCol_PopupBg] = GetImVec4FromGlmVec4(theme.Colors.PopupBg);
	style.Colors[ImGuiCol_Border] = GetImVec4FromGlmVec4(theme.Colors.Border);
	style.Colors[ImGuiCol_BorderShadow] = GetImVec4FromGlmVec4(theme.Colors.BorderShadow);
	style.Colors[ImGuiCol_FrameBg] = GetImVec4FromGlmVec4(theme.Colors.FrameBg);
	style.Colors[ImGuiCol_FrameBgHovered] = GetImVec4FromGlmVec4(theme.Colors.FrameBgHovered);
	style.Colors[ImGuiCol_FrameBgActive] = GetImVec4FromGlmVec4(theme.Colors.FrameBgActive);
	style.Colors[ImGuiCol_TitleBg] = GetImVec4FromGlmVec4(theme.Colors.TitleBg);
	style.Colors[ImGuiCol_TitleBgActive] = GetImVec4FromGlmVec4(theme.Colors.TitleBgActive);
	style.Colors[ImGuiCol_TitleBgCollapsed] = GetImVec4FromGlmVec4(theme.Colors.TitleBgCollapsed);
	style.Colors[ImGuiCol_MenuBarBg] = GetImVec4FromGlmVec4(theme.Colors.MenuBarBg);
	style.Colors[ImGuiCol_ScrollbarBg] = GetImVec4FromGlmVec4(theme.Colors.ScrollbarBg);
	style.Colors[ImGuiCol_ScrollbarGrab] = GetImVec4FromGlmVec4(theme.Colors.ScrollbarGrab);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = GetImVec4FromGlmVec4(theme.Colors.ScrollbarGrabHovered);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = GetImVec4FromGlmVec4(theme.Colors.ScrollbarGrabActive);
	style.Colors[ImGuiCol_CheckMark] = GetImVec4FromGlmVec4(theme.Colors.CheckMark);
	style.Colors[ImGuiCol_SliderGrab] = GetImVec4FromGlmVec4(theme.Colors.SliderGrab);
	style.Colors[ImGuiCol_SliderGrabActive] = GetImVec4FromGlmVec4(theme.Colors.SliderGrabActive);
	style.Colors[ImGuiCol_Button] = GetImVec4FromGlmVec4(theme.Colors.Button);
	style.Colors[ImGuiCol_ButtonHovered] = GetImVec4FromGlmVec4(theme.Colors.ButtonHovered);
	style.Colors[ImGuiCol_ButtonActive] = GetImVec4FromGlmVec4(theme.Colors.ButtonActive);
	style.Colors[ImGuiCol_Header] = GetImVec4FromGlmVec4(theme.Colors.Header);
	style.Colors[ImGuiCol_HeaderHovered] = GetImVec4FromGlmVec4(theme.Colors.HeaderHovered);
	style.Colors[ImGuiCol_HeaderActive] = GetImVec4FromGlmVec4(theme.Colors.HeaderActive);
	style.Colors[ImGuiCol_Separator] = GetImVec4FromGlmVec4(theme.Colors.Separator);
	style.Colors[ImGuiCol_SeparatorHovered] = GetImVec4FromGlmVec4(theme.Colors.SeparatorHovered);
	style.Colors[ImGuiCol_SeparatorActive] = GetImVec4FromGlmVec4(theme.Colors.SeparatorActive);
	style.Colors[ImGuiCol_ResizeGrip] = GetImVec4FromGlmVec4(theme.Colors.ResizeGrip);
	style.Colors[ImGuiCol_ResizeGripHovered] = GetImVec4FromGlmVec4(theme.Colors.ResizeGripHovered);
	style.Colors[ImGuiCol_ResizeGripActive] = GetImVec4FromGlmVec4(theme.Colors.ResizeGripActive);
	style.Colors[ImGuiCol_Tab] = GetImVec4FromGlmVec4(theme.Colors.Tab);
	style.Colors[ImGuiCol_TabHovered] = GetImVec4FromGlmVec4(theme.Colors.TabHovered);
	style.Colors[ImGuiCol_TabActive] = GetImVec4FromGlmVec4(theme.Colors.TabActive);
	style.Colors[ImGuiCol_TabUnfocused] = GetImVec4FromGlmVec4(theme.Colors.TabUnfocused);
	style.Colors[ImGuiCol_TabUnfocusedActive] = GetImVec4FromGlmVec4(theme.Colors.TabUnfocusedActive);
	style.Colors[ImGuiCol_PlotLines] = GetImVec4FromGlmVec4(theme.Colors.PlotLines);
	style.Colors[ImGuiCol_PlotLinesHovered] = GetImVec4FromGlmVec4(theme.Colors.PlotLinesHovered);
	style.Colors[ImGuiCol_PlotHistogram] = GetImVec4FromGlmVec4(theme.Colors.PlotHistogram);
	style.Colors[ImGuiCol_PlotHistogramHovered] = GetImVec4FromGlmVec4(theme.Colors.PlotHistogramHovered);
	style.Colors[ImGuiCol_TableHeaderBg] = GetImVec4FromGlmVec4(theme.Colors.TableHeaderBg);
	style.Colors[ImGuiCol_TableBorderStrong] = GetImVec4FromGlmVec4(theme.Colors.TableBorderStrong);
	style.Colors[ImGuiCol_TableBorderLight] = GetImVec4FromGlmVec4(theme.Colors.TableBorderLight);
	style.Colors[ImGuiCol_TableRowBg] = GetImVec4FromGlmVec4(theme.Colors.TableRowBg);
	style.Colors[ImGuiCol_TableRowBgAlt] = GetImVec4FromGlmVec4(theme.Colors.TableRowBgAlt);
	style.Colors[ImGuiCol_TextSelectedBg] = GetImVec4FromGlmVec4(theme.Colors.TextSelectedBg);
	style.Colors[ImGuiCol_DragDropTarget] = GetImVec4FromGlmVec4(theme.Colors.DragDropTarget);
	style.Colors[ImGuiCol_NavHighlight] = GetImVec4FromGlmVec4(theme.Colors.NavHighlight);
	style.Colors[ImGuiCol_NavWindowingHighlight] = GetImVec4FromGlmVec4(theme.Colors.NavWindowingHighlight);
	style.Colors[ImGuiCol_NavWindowingDimBg] = GetImVec4FromGlmVec4(theme.Colors.NavWindowingDimBg);
	style.Colors[ImGuiCol_ModalWindowDimBg] = GetImVec4FromGlmVec4(theme.Colors.ModalWindowDimBg);

	Log::Trace("ThemeManager::Apply - Theme has been applied");
}

ImVec2 ThemeManager::GetImVec2FromGlmVec2(const glm::vec2& vec) {
	return ImVec2(vec.x, vec.y);
}

ImVec4 ThemeManager::GetImVec4FromGlmVec4(const glm::vec4& vec) {
	return ImVec4(vec.r, vec.g, vec.b, vec.a);
}

ImGuiDir ThemeManager::GetImGuiDirFromDirection(const Direction& direction) {
	switch (direction) {
		case Direction::None:	return ImGuiDir_None;
		case Direction::Left:	return ImGuiDir_Left;
		case Direction::Right:	return ImGuiDir_Right;
		case Direction::Up:		return ImGuiDir_Up;
		case Direction::Down:	return ImGuiDir_Down;
		default:				return ImGuiDir_None;
	}
}
