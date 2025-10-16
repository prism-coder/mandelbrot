#include "SettingsWindow.h"

#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Settings/SettingsManager.h"

#include "Utilities/Utilities.h"

#include "Editor/UI.h"
#include "Editor/Theme/ThemeManager.h"

#include <algorithm>

SettingsWindow::SettingsWindow(bool& isOpen)
	: BaseWindow(isOpen)
{}

void SettingsWindow::OnAttach() {
	Log::Trace("SettingsWindow::OnAttach - Attaching Settings Window");

	m_Categories = {
		Category::Application,
		Category::Editor,
		//Category::Graphics,
		//Category::Input,
		//Category::Localization,
		//Category::Quality,
		Category::Rendering,
		//Category::Time
	};

	m_Themes = {
		EditorTheme::AdobeInspired,
		EditorTheme::AyuDark,
		EditorTheme::BessDark,
		EditorTheme::BlackDevil,
		EditorTheme::BootstrapDark,
		EditorTheme::Carbon,
		EditorTheme::Cherno,
		EditorTheme::Cherry,
		EditorTheme::ClassicSteam,
		EditorTheme::Classic,
		EditorTheme::ComfortableDarkCyan,
		EditorTheme::ComfortableLightOrange,
		EditorTheme::Comfy,
		EditorTheme::Darcula,
		EditorTheme::DarkRed,
		EditorTheme::DarkRuda,
		EditorTheme::Dark,
		EditorTheme::Darky,
		EditorTheme::DeepDark,
		EditorTheme::DiscordDark,
		EditorTheme::Enemymouse,
		EditorTheme::Everforest,
		EditorTheme::Excellency,
		EditorTheme::FutureDark,
		EditorTheme::Gold,
		EditorTheme::GreenFont,
		EditorTheme::GreenLeaf,
		EditorTheme::HazyDark,
		EditorTheme::LedSynthmaster,
		EditorTheme::Light,
		EditorTheme::MaterialFlat,
		EditorTheme::Microsoft,
		EditorTheme::Modern,
		EditorTheme::Photoshop,
		EditorTheme::PurpleComfy,
		EditorTheme::QuickMinimalLook,
		EditorTheme::RedFont,
		EditorTheme::RedOni,
		EditorTheme::Rest,
		EditorTheme::RoundedVisualStudio,
		EditorTheme::SoftCherry,
		EditorTheme::SonicRiders,
		EditorTheme::Unreal,
		EditorTheme::VisualStudio,
		EditorTheme::Windark
	};

	m_RenderingEngines = {
		RenderingEngine::OpenGL,
		RenderingEngine::DirectX,
		RenderingEngine::Vulkan
	};
}

void SettingsWindow::OnDetach() {
	Log::Trace("SettingsWindow::OnDetach - Detaching Settings Window");
}

void SettingsWindow::OnUpdate(Timestep ts) {}

void SettingsWindow::OnUIRender() {
	if (!m_IsOpen) {
		return;
	}

	if (ImGui::Begin("Settings", &m_IsOpen)) {
		if (ImGui::BeginTable("##SettingsWindowTable", 2, ImGuiTableFlags_Resizable)) {
			if (ImGui::TableNextColumn()) {
				if (ImGui::BeginChild("##SettingsSidebar", { 0, 0 }, ImGuiChildFlags_Borders)) {
					const auto& style = ImGui::GetStyle();
					for (const auto& category : m_Categories) {
						if (ImGui::Selectable(Utilities::CategoryToString(category).c_str(), m_CurrentCategory == category)) {
							m_CurrentCategory = category;
						}
					}
				}

				ImGui::EndChild();
			}

			if (ImGui::TableNextColumn()) {
				const auto& style = ImGui::GetStyle();
				float footerHeight =
					ImGui::GetFrameHeight()
					+ style.WindowPadding.y * 2.0f
					+ style.ChildBorderSize * 2.0f
					+ style.ItemSpacing.y * 2.0f;

				if (ImGui::BeginChild("##SettingsContent", { 0, -footerHeight }, ImGuiChildFlags_Borders)) {
					DrawSettings();
				}

				ImGui::EndChild();

				UI::Separator();

				if (ImGui::BeginChild("##SettingsFooter", { 0, 0 }, ImGuiChildFlags_Borders)) {
					float buttonWidth = 100.0f;
					float totalButtonWidth = buttonWidth * 2 + style.ItemSpacing.x;
					float indent = (ImGui::GetContentRegionAvail().x - totalButtonWidth) * 0.5f;

					if (indent > 0) {
						ImGui::Indent(indent);
					}

					if (UI::Button("Save", { buttonWidth , 0 })) {
						SettingsManager::Save();
					}

					ImGui::SameLine();

					if (UI::Button("Load", { buttonWidth , 0 })) {
						SettingsManager::Load();
					}

					if (indent > 0) {
						ImGui::Unindent(indent);
					}
				}

				ImGui::EndChild();
			}

			ImGui::EndTable();
		}
	}

	ImGui::End();
}

void SettingsWindow::DrawSettings() {
	Settings& settings = SettingsManager::Get();

	switch (m_CurrentCategory) {
		case Category::Application: DrawApplicationSettings(settings.Application);	break;
		case Category::Editor:		DrawEditorSettings(settings.Editor);			break;
		case Category::Rendering:	DrawRenderingSettings(settings.Rendering);		break;
		default:					DrawWIP();										break;
	}
}

void SettingsWindow::DrawApplicationSettings(ApplicationSettings& application) {
	UI::DisabledInputText("Name", application.Name);
	UI::Tooltip("The name of the Application.");

	UI::ConfigurationSlot("Startup Configuration", application.StartupConfiguration);
	UI::Tooltip("The startup Fractal Configuration.\nUse the Project Window to drag a Startup Configuration from the Assets Folder.\nOr set it up manually through the 'Settings.yaml' file.");

	const auto& version = application.Version;

	std::string versionStr = (
		std::to_string(version.Major) + "." +
		std::to_string(version.Minor) + "." +
		std::to_string(version.Patch)
	);

	UI::DisabledInputText("Version", versionStr);
	UI::Tooltip("The version of the Application.");

	UI::Bool("Maximized", application.Maximized);
	UI::Tooltip("Whether the Application will start Maximized or not.");

	UI::Bool("Debug Mode", application.DebugMode);
	UI::Tooltip("When the Debug Mode is activated, a .log file will be written on the root of the Application folder.");
}

void SettingsWindow::DrawEditorSettings(EditorSettings& editor) {
	if (UI::CollapsingHeader("Appearance")) {
		auto& appearance = editor.Appearance;

		if (UI::Dropdown("Theme", m_Themes, appearance.Theme, Utilities::EditorThemeToString)) {
			if (ThemeManager::Load(ThemeManager::Select(appearance.Theme))) {
				ThemeManager::Apply(ThemeManager::Get());
			}
		}
		UI::Tooltip("Set the Editor Theme.");

		UI::DragInt("Font Size", appearance.FontSize, 0, 1638);
		UI::Tooltip("To change the Font Size you need to restart the Editor.");

		if (UI::DragFloat("UI Scale", appearance.UIScale, 0.01f, 10.0f)) {
			auto& style = ImGui::GetStyle();
			style.FontScaleMain = appearance.UIScale;
		}
		UI::Tooltip("Change the UI Scale.");

		UI::DragFloat("Column Width", appearance.ColumnWidth, 0.0f, 300.0f);
		UI::Tooltip("Changes the column width for the Property Grid Name Column");

		UI::Separator();
	}

	UI::DragInt("Auto Save Interval", editor.AutoSaveInterval, 0, 86400);
	UI::Tooltip("Auto Save is not supported yet.");
}

void SettingsWindow::DrawRenderingSettings(RenderingSettings& rendering) {
	UI::Dropdown("Engine", m_RenderingEngines, rendering.Engine, Utilities::RenderingEngineToString);
	UI::Tooltip("Select a Rendering API.\nCurrently, only OpenGL is supported.");

	if (UI::CollapsingHeader("Resolution")) {
		auto& resolution = rendering.Resolution;

		UI::DragInt("Width", resolution.Width, 1);
		UI::Tooltip("The width of the Application.");

		UI::DragInt("Height", resolution.Height, 1);
		UI::Tooltip("The height of the Application.");

		UI::Separator();

		UI::SliderFloat("Scale", resolution.Scale, 0.01f, 10.0f);
		UI::Tooltip("The scale of the Rendering on the Viewport Window.\nUse at your own risk.");

		UI::Separator();
	}

	UI::Bool("Fullscreen", rendering.Fullscreen);
	UI::Tooltip("Whether the Application will start in Full Screen mode.");

	UI::Bool("VSync", rendering.VSync);
	UI::Tooltip("Whether VSync is activated or not.");
}

void SettingsWindow::DrawWIP() {
	ImGui::Text("Work in progress");
}
