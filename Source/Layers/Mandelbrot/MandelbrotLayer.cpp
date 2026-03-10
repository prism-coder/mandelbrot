#include "MandelbrotLayer.h"

#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Settings/SettingsManager.h"
#include "Core/Input/Input.h"

#include "Renderer/Renderer.h"

#include "Editor/Windows.h"
#include "Editor/UI.h"

#include "MandelbrotSerializer.h"

#include <cstring>

MandelbrotLayer::MandelbrotLayer() {
	m_Windows.emplace_back(CreateScope<AboutWindow>(SettingsManager::Get().Editor.Windows.ShowAbout));
	m_Windows.emplace_back(CreateScope<InspectorWindow>(SettingsManager::Get().Editor.Windows.ShowInspector, m_FractalState));

	auto projectWindow = CreateScope<ProjectWindow>(SettingsManager::Get().Editor.Windows.ShowProject);
	projectWindow->SetConfigurationLoadCallback([this](const std::filesystem::path& filepath) {
		LoadConfiguration(filepath);
	});
	m_Windows.emplace_back(std::move(projectWindow));

	m_Windows.emplace_back(CreateScope<SettingsWindow>(SettingsManager::Get().Editor.Windows.ShowSettings));
	m_Windows.emplace_back(CreateScope<StatisticsWindow>(SettingsManager::Get().Editor.Windows.ShowStatistics));

	auto viewportWindow = CreateScope<ViewportWindow>(SettingsManager::Get().Editor.Windows.ShowViewport, m_FractalState);
	viewportWindow->SetConfigurationLoadCallback([this](const std::filesystem::path& filepath) {
		LoadConfiguration(filepath);
	});
	m_Windows.emplace_back(std::move(viewportWindow));
}

void MandelbrotLayer::OnAttach() {
	Log::Trace("EditorLayer::OnAttach - Attaching Editor Layer");

	m_FractalState.Current = m_FractalState.Target;

	for (const auto& window : m_Windows) {
		window->OnAttach();
	}

	const auto& settings = SettingsManager::Get();

	m_LastWindowsSettings = settings.Editor.Windows;

	if (settings.Application.StartupConfiguration.empty()) {
		LoadConfiguration(m_DefaultConfigurationFilepath);
	} else {
		LoadConfiguration(settings.Application.StartupConfiguration);
	}
}

void MandelbrotLayer::OnDetach() {
	Log::Trace("EditorLayer::OnDetach - Detaching Editor Layer");

	for (const auto& window : m_Windows) {
		window->OnDetach();
	}

	m_Windows.clear();
}

void MandelbrotLayer::OnUpdate(Timestep ts) {
	// Auto-save when any window visibility changes.
	const auto& currentWindows = SettingsManager::Get().Editor.Windows;
	if (std::memcmp(&currentWindows, &m_LastWindowsSettings, sizeof(WindowsSettings)) != 0) {
		m_LastWindowsSettings = currentWindows;
		SettingsManager::Save();
	}

	m_FractalState.Update(ts, SettingsManager::Get().Navigation);

	for (const auto& window : m_Windows) {
		window->OnUpdate(ts);
	}

	HandleKeyboardShortcuts();

	if (m_RequestExport) {
		ExportFrameAsImage();
		m_RequestExport = false;
	}
}

void MandelbrotLayer::OnUIRender() {
	m_ConfigurationFilepathToLoad.clear();

	DrawMenuBar();

	for (const auto& window : m_Windows) {
		window->OnUIRender();
	}

	if (!m_ConfigurationFilepathToLoad.empty()) {
		LoadConfiguration(m_ConfigurationFilepathToLoad);
	}
}

void MandelbrotLayer::DrawMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		// File Menu
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New", "Ctrl+N")) {

			}

			if (ImGui::MenuItem("Save", "Ctrl+S")) {
				SaveConfiguration(m_CurrentConfigurationFilepath);
			}

			if (ImGui::BeginMenu("Open Recent", !m_RecentConfigurationFilepaths.empty())) {
				for (const auto& configurationFilepath : m_RecentConfigurationFilepaths) {
					if (!configurationFilepath.filename().empty()) {
						if (ImGui::MenuItem(configurationFilepath.filename().replace_extension("").string().c_str())) {
							m_ConfigurationFilepathToLoad = configurationFilepath;
							break;
						}
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Presets")) {
				DrawPresetsRecursive(m_PresetsFilepath);

				ImGui::EndMenu();
			}

			UI::Separator();

			ImGui::MenuItem("Settings", "Ctrl+,", &SettingsManager::Get().Editor.Windows.ShowSettings);

			UI::Separator();

			if (ImGui::MenuItem("Exit", "Alt+F4")) {
				Application::Get().Close();
			}

			ImGui::EndMenu();
		}

		// Edit Menu
		/*if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			if (ImGui::MenuItem("Delete", "DEL")) {}
			ImGui::EndMenu();
		}*/

		// Export Menu
		if (ImGui::BeginMenu("Export")) {
			const auto& fmt = SettingsManager::Get().Export.ImageFormat;
			std::string imageLabel = "Image (." + [&fmt]() -> std::string {
				switch (fmt) {
					case ExportImageFormat::JPEG: return "jpg";
					case ExportImageFormat::BMP:  return "bmp";
					default:                      return "png";
				}
			}() + ")";

			if (ImGui::MenuItem(imageLabel.c_str())) {
				m_RequestExport = true;
			}

			UI::Tooltip("Export the current frame as an image to the Export/Image folder.");

			if (ImGui::MenuItem("Configuration (.fractal)")) {
				ExportConfiguration();
			}

			UI::Tooltip("Export the current fractal configuration to the 'Export/Configuration' folder.");

			ImGui::EndMenu();
		}

		// View Menu
		if (ImGui::BeginMenu("View")) {
			auto& windowsSettings = SettingsManager::Get().Editor.Windows;

			ImGui::MenuItem("About",     "F1",     &windowsSettings.ShowAbout);
			ImGui::MenuItem("Inspector", "Ctrl+I", &windowsSettings.ShowInspector);
			ImGui::MenuItem("Project",   "Ctrl+P", &windowsSettings.ShowProject);
			ImGui::MenuItem("Settings",  "Ctrl+,", &windowsSettings.ShowSettings);
			ImGui::MenuItem("Statistics","Ctrl+T", &windowsSettings.ShowStatistics);
			ImGui::MenuItem("Viewport",  "Ctrl+V", &windowsSettings.ShowViewport);

			ImGui::EndMenu();
		}

		// Help Menu
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("Documentation")) {
				UI::OpenLink("https://github.com/prism-coder/mandelbrot");
			}

			UI::Tooltip("Open the app documentation.");

			if (ImGui::MenuItem("About")) {
				SettingsManager::Get().Editor.Windows.ShowAbout = true;
			}

			UI::Tooltip("Open the about window.");

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void MandelbrotLayer::DrawPresetsRecursive(const std::filesystem::path& directoryPath) {
	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
		const auto& path = entry.path();

		if (entry.is_directory()) {
			// If the entry is a directory, create a SUB-MENU for it
			if (ImGui::BeginMenu(path.filename().string().c_str())) {
				// And call this same function to fill that sub-menu
				DrawPresetsRecursive(path);
				ImGui::EndMenu(); // Important to close the sub-menu
			}
		} else {
			// If it is a file, we check that it is a preset
			if (!path.filename().empty() && path.extension() == ".fractal") {
				// We created a normal `MenuItem`. We used replace_extension for a cleaner name.
				if (ImGui::MenuItem(path.filename().replace_extension("").string().c_str())) {
					m_ConfigurationFilepathToLoad = path;
				}
			}
		}
	}
}

bool MandelbrotLayer::NewConfiguration(const std::string& name, const std::filesystem::path& filepath) {
	Log::Info("MandelbrotLayer::NewConfiguration - Creating new Configuration");

	m_CurrentConfigurationFilepath.clear();

	Application::Get().SetWindowTitle(name);

	if (SaveConfiguration(filepath)) {
		Log::Info("MandelbrotLayer::NewConfiguration - New Configuration has been created");

		return true;
	}

	Log::Warning("MandelbrotLayer::NewConfiguration - Couldn't create new Configuration");

	return false;
}

bool MandelbrotLayer::SaveConfiguration(const std::filesystem::path& filepath) {
	Log::Info("MandelbrotLayer::SaveConfiguration - Saving Configuration to " + filepath.string());

	MandelbrotSerializer serializer(m_FractalState.Target);

	if (serializer.Serialize(filepath)) {
		Log::Info("MandelbrotLayer::SaveConfiguration - Configuration has been saved");

		return true;
	}
	
	Log::Warning("MandelbrotLayer::SaveConfiguration - Couldn't save Configuration");

	return false;
}

bool MandelbrotLayer::LoadConfiguration(const std::filesystem::path& filepath) {
	if (m_CurrentConfigurationFilepath == filepath) {
		Log::Warning("MandelbrotLayer::LoadConfiguration - Configuration is already loaded: " + filepath.string());
		return false;
	}

	m_CurrentConfigurationFilepath = filepath;

	Log::Info("MandelbrotLayer::LoadConfiguration - Loading Configuration from " + filepath.string());

	MandelbrotSerializer serializer(m_FractalState.Target);

	if (serializer.Deserialize(filepath)) {
		Log::Info("MandelbrotLayer::LoadConfiguration - Configuration has been loaded");
		UpdateWindowTitle(filepath);
		AddToRecentConfigurations(filepath);
		return true;
	}

	Log::Warning("MandelbrotLayer::LoadConfiguration - Couldn't load Configuration");

	return false;
}

std::filesystem::path MandelbrotLayer::BuildExportPath(const std::filesystem::path& folder, const std::string& extension) {
	const std::filesystem::path exportRoot = SettingsManager::Get().Export.Folder;
	CheckOrCreateFolder(exportRoot);
	CheckOrCreateFolder(folder);

	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	std::stringstream ss;
	ss << "Mandelbrot-" << std::put_time(std::localtime(&in_time_t), "%Y%m%d-%H%M%S") << extension;

	return folder / ss.str();
}

void MandelbrotLayer::ExportFrameAsImage() {
	const auto& exportSettings = SettingsManager::Get().Export;

	std::string ext;
	switch (exportSettings.ImageFormat) {
		case ExportImageFormat::JPEG:	ext = ".jpg";	break;
		case ExportImageFormat::BMP:	ext = ".bmp";	break;
		case ExportImageFormat::PNG:
		default:						ext = ".png";	break;
	}

	const std::filesystem::path exportImageFolder = exportSettings.Folder / "Image";
	auto filepath = BuildExportPath(exportImageFolder, ext);
	Renderer::ExportFrame(filepath);
	Log::Info("MandelbrotLayer::ExportFrameAsImage - Frame exported successfully to: " + filepath.string());
}

void MandelbrotLayer::ExportConfiguration() {
	const std::filesystem::path exportConfigFolder = SettingsManager::Get().Export.Folder / "Configuration";
	SaveConfiguration(BuildExportPath(exportConfigFolder, ".fractal"));
}

void MandelbrotLayer::CheckOrCreateFolder(const std::filesystem::path& filepath) {
	if (!std::filesystem::exists(filepath)) {
		std::filesystem::create_directory(filepath);
	}
}

void MandelbrotLayer::HandleKeyboardShortcuts() {
	if (Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl)) {
		if (Input::IsKeyDown(KeyCode::S)) {
			SaveConfiguration(m_CurrentConfigurationFilepath);
		}

		if (Input::IsKeyDown(KeyCode::N)) {
			NewConfiguration("Untitled", m_CurrentConfigurationFilepath);
		}

		if (Input::IsKeyDown(KeyCode::P)) {
			SettingsManager::Get().Editor.Windows.ShowProject = !SettingsManager::Get().Editor.Windows.ShowProject;
		}

		if (Input::IsKeyDown(KeyCode::Comma)) {
			SettingsManager::Get().Editor.Windows.ShowSettings = !SettingsManager::Get().Editor.Windows.ShowSettings;
		}

		if (Input::IsKeyDown(KeyCode::I)) {
			SettingsManager::Get().Editor.Windows.ShowInspector = !SettingsManager::Get().Editor.Windows.ShowInspector;
		}

		if (Input::IsKeyDown(KeyCode::T)) {
			SettingsManager::Get().Editor.Windows.ShowStatistics = !SettingsManager::Get().Editor.Windows.ShowStatistics;
		}

		if (Input::IsKeyDown(KeyCode::V)) {
			SettingsManager::Get().Editor.Windows.ShowViewport = !SettingsManager::Get().Editor.Windows.ShowViewport;
		}
	}

	if (Input::IsKeyDown(KeyCode::F1)) {
		SettingsManager::Get().Editor.Windows.ShowAbout = !SettingsManager::Get().Editor.Windows.ShowAbout;
	}
}

void MandelbrotLayer::UpdateWindowTitle(const std::filesystem::path& filepath) {
	Application::Get().SetWindowTitle(filepath.filename().replace_extension("").string());
}

void MandelbrotLayer::AddToRecentConfigurations(const std::filesystem::path& filepath) {
	auto it = std::find(m_RecentConfigurationFilepaths.begin(), m_RecentConfigurationFilepaths.end(), filepath);
	if (it != m_RecentConfigurationFilepaths.end()) {
		m_RecentConfigurationFilepaths.erase(it);
	}

	m_RecentConfigurationFilepaths.push_back(filepath);
}
