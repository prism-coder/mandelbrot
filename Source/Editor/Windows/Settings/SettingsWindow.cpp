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
		Category::Export,
		//Category::Graphics,
		Category::Input,
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

	m_WindowModes = {
		WindowMode::Windowed,
		WindowMode::Fullscreen,
		WindowMode::Borderless
	};

	m_ExportImageFormats = {
		ExportImageFormat::PNG,
		ExportImageFormat::JPEG,
		ExportImageFormat::BMP
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
		case Category::Input:		DrawNavigationSettings(settings.Navigation);	break;
		case Category::Rendering:	DrawRenderingSettings(settings.Rendering);		break;
		case Category::Export:		DrawExportSettings(settings.Export);			break;
		default:					DrawWIP();										break;
	}
}

void SettingsWindow::DrawApplicationSettings(ApplicationSettings& applicationSettings) {
	UI::DisabledInputText("Name", applicationSettings.Name);
	UI::Tooltip("The name of the Application.");

	UI::ConfigurationSlot("Startup Configuration", applicationSettings.StartupConfiguration);
	UI::Tooltip("The startup Fractal Configuration.\nUse the Project Window to drag a Startup Configuration from the Assets Folder.\nOr set it up manually through the 'Settings.yaml' file.");

	UI::Separator();

	const auto& versionSettings = applicationSettings.Version;

	std::string versionStr = versionSettings.GetVersion();

	UI::DisabledInputText("Version", versionStr);
	UI::Tooltip("The version of the Application.");

	UI::Separator();

	UI::Bool("Maximized", applicationSettings.Maximized);
	UI::Tooltip("Whether the Application will start Maximized or not.");

	UI::Bool("Debug Mode", applicationSettings.DebugMode);
	UI::Tooltip("When the Debug Mode is activated, additional diagnostic information may be shown.");

	UI::Bool("Log to File", applicationSettings.LogToFile);
	UI::Tooltip("When enabled, all log output is also written to 'Application.log' on disk.");

	UI::Bool("Escape Closes App", applicationSettings.EscapeClosesApp);
	UI::Tooltip("When enabled, pressing Escape will close the application.");

	UI::Separator();
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

	UI::Separator();
}

void SettingsWindow::DrawExportSettings(ExportSettings& exportSettings) {
	UI::Dropdown("Image Format", m_ExportImageFormats, exportSettings.ImageFormat, Utilities::ExportImageFormatToString);
	UI::Tooltip("The image format used when exporting a frame.");

	if (exportSettings.ImageFormat == ExportImageFormat::JPEG) {
		UI::SliderInt("Image Quality", exportSettings.ImageQuality, 0, 100);
		UI::Tooltip("JPEG compression quality (0 = smallest file, 100 = best quality).");
	}

	std::string folderStr = exportSettings.Folder.string();
	if (UI::InputText("Export Folder", folderStr)) {
		exportSettings.Folder = folderStr;
	}
	UI::Tooltip("Root folder where exported images and configurations are saved.");

	UI::Separator();
}

void SettingsWindow::DrawNavigationSettings(NavigationSettings& navigation) {
	UI::DragFloat("Movement Speed", navigation.MovementSpeed, 0.1f, 50.0f);
	UI::Tooltip("Pan speed when using WASD or dragging the viewport.");

	UI::DragFloat("Rotation Speed", navigation.RotationSpeed, 0.1f, 50.0f);
	UI::Tooltip("Rotation speed when using the Q and E keys.");

	UI::DragFloat("Zoom Speed", navigation.ZoomSpeed, 0.1f, 50.0f);
	UI::Tooltip("Zoom speed when using the scroll wheel, Shift or Ctrl.");

	UI::DragFloat("Power Speed", navigation.PowerSpeed, 0.1f, 50.0f);
	UI::Tooltip("Speed when changing the fractal power with PageUp / PageDown.");

	UI::Separator();

	UI::DragFloat("Smoothing", navigation.Smoothing, 0.1f, 50.0f);
	UI::Tooltip("Interpolation smoothing factor.\nHigher values produce snappier, faster transitions.");

	UI::Separator();

	UI::Bool("Invert Zoom", navigation.InvertZoom);
	UI::Tooltip("Reverses the scroll wheel zoom direction.");

	UI::Separator();
}

void SettingsWindow::DrawRenderingSettings(RenderingSettings& rendering) {
	UI::Dropdown("Engine", m_RenderingEngines, rendering.Engine, Utilities::RenderingEngineToString);
	UI::Tooltip("Select a Rendering API.\nCurrently, only OpenGL is supported.");

	UI::Dropdown("Window Mode", m_WindowModes, rendering.Mode, Utilities::WindowModeToString);
	UI::Tooltip("Windowed: standard window.\nFullscreen: exclusive fullscreen.\nBorderless: borderless window covering the screen.");

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

	UI::Bool("VSync", rendering.VSync);
	UI::Tooltip("Whether VSync is activated or not.");

	UI::Bool("Lock Framerate", rendering.LockFramerate);
	UI::Tooltip("When enabled, the frame rate will be capped to the value specified in 'Target Frame Rate'.");

	if (rendering.LockFramerate) {
		UI::DragInt("Target Frame Rate", rendering.TargetFrameRate, 1, 960);
		UI::Tooltip("Frame rate cap in FPS. Set to 0 for uncapped rendering.");
	}

	UI::Separator();
}

void SettingsWindow::DrawWIP() {
	ImGui::Text("Work in progress");
}
