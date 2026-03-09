#include "MandelbrotLayer.h"

#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Settings/SettingsManager.h"
#include "Core/Input/Input.h"

#include "Renderer/Renderer.h"

#include "Editor/Windows.h"
#include "Editor/UI.h"

#include "MandelbrotSerializer.h"

MandelbrotLayer::MandelbrotLayer() {
	m_Windows.emplace_back(CreateScope<AboutWindow>(m_AboutWindowOpen));
	m_Windows.emplace_back(CreateScope<InspectorWindow>(m_InspectorWindowOpen, m_FractalState));

	auto projectWindow = CreateScope<ProjectWindow>(m_ProjectWindowOpen);
	projectWindow->SetConfigurationLoadCallback([this](const std::filesystem::path& filepath) {
		LoadConfiguration(filepath);
	});
	m_Windows.emplace_back(std::move(projectWindow));

	m_Windows.emplace_back(CreateScope<SettingsWindow>(m_SettingsWindowOpen));
	m_Windows.emplace_back(CreateScope<StatisticsWindow>(m_StatisticsWindowOpen));

	auto viewportWindow = CreateScope<ViewportWindow>(m_ViewportWindowOpen, m_FractalState);
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

	const auto& settings = SettingsManager::Get().Application;

	if (settings.StartupConfiguration.empty()) {
		LoadConfiguration(m_DefaultConfigurationFilepath);
	} else {
		LoadConfiguration(settings.StartupConfiguration);
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
	m_FractalState.Update(ts);

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

			if (ImGui::MenuItem("Settings", "Ctrl+,")) {
				m_SettingsWindowOpen = true;
			}

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
			if (ImGui::MenuItem("Image (.png)")) {
				m_RequestExport = true;
			}

			UI::Tooltip("Export the current frame as a PNG image to the 'Export/Image' folder.");

			if (ImGui::MenuItem("Configuration (.fractal)")) {
				ExportConfiguration();
			}

			UI::Tooltip("Export the current fractal configuration to the 'Export/Configuration' folder.");

			ImGui::EndMenu();
		}

		// View Menu
		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("About", "F1", &m_AboutWindowOpen);
			ImGui::MenuItem("Inspector", "Ctrl+I", &m_InspectorWindowOpen);
			ImGui::MenuItem("Project", "Ctrl+P", &m_ProjectWindowOpen);
			ImGui::MenuItem("Settings", "Ctrl+,", &m_SettingsWindowOpen);
			ImGui::MenuItem("Statistics", "Ctrl+T", &m_StatisticsWindowOpen);
			ImGui::MenuItem("Viewport", "Ctrl+V", &m_ViewportWindowOpen);

			ImGui::EndMenu();
		}

		// Help Menu
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("Documentation")) {
				UI::OpenLink("https://github.com/prism-coder/mandelbrot");
			}

			UI::Tooltip("Open the app documentation.");

			if (ImGui::MenuItem("About")) {
				m_AboutWindowOpen = true;
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
	CheckOrCreateFolder(m_ExportFilepath);
	CheckOrCreateFolder(folder);

	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	std::stringstream ss;
	ss << "Mandelbrot-" << std::put_time(std::localtime(&in_time_t), "%Y%m%d-%H%M%S") << extension;

	return folder / ss.str();
}

void MandelbrotLayer::ExportFrameAsImage() {
	auto filepath = BuildExportPath(m_ExportImageFilepath, ".png");
	Renderer::ExportFrame(filepath);
	Log::Info("MandelbrotLayer::ExportFrameAsImage - Frame exported successfully to: " + filepath.string());
}

void MandelbrotLayer::ExportConfiguration() {
	SaveConfiguration(BuildExportPath(m_ExportConfigurationFilepath, ".fractal"));
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
			m_ProjectWindowOpen = !m_ProjectWindowOpen;
		}

		if (Input::IsKeyDown(KeyCode::Comma)) {
			m_SettingsWindowOpen = !m_SettingsWindowOpen;
		}

		if (Input::IsKeyDown(KeyCode::I)) {
			m_InspectorWindowOpen = !m_InspectorWindowOpen;
		}

		if (Input::IsKeyDown(KeyCode::T)) {
			m_StatisticsWindowOpen = !m_StatisticsWindowOpen;
		}

		if (Input::IsKeyDown(KeyCode::V)) {
			m_ViewportWindowOpen = !m_ViewportWindowOpen;
		}
	}

	if (Input::IsKeyDown(KeyCode::F1)) {
		m_AboutWindowOpen = !m_AboutWindowOpen;
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
