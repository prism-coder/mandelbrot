#pragma once

#include "Core/Core.h"
#include "Core/Layer.h"

#include "Editor/BaseWindow.h"

#include "FractalState.h"

#include <vector>
#include <filesystem>

class MandelbrotLayer : public Layer {
public:
	MandelbrotLayer();
	virtual ~MandelbrotLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnUIRender() override;
private:
	void DrawMenuBar();
	void DrawPresetsRecursive(const std::filesystem::path& directoryPath);

	bool NewConfiguration(const std::string& name, const std::filesystem::path& filepath);
	bool SaveConfiguration(const std::filesystem::path& filepath);
	bool LoadConfiguration(const std::filesystem::path& filepath);

	void ExportFrameAsImage();
	void ExportConfiguration();

	std::filesystem::path BuildExportPath(const std::filesystem::path& folder, const std::string& extension);
	void CheckOrCreateFolder(const std::filesystem::path& filepath);

	void HandleKeyboardShortcuts();
	void UpdateWindowTitle(const std::filesystem::path& filepath);
	void AddToRecentConfigurations(const std::filesystem::path& filepath);
private:
	// Export Request
	bool m_RequestExport = false;

	// Window visibility snapshot for auto-save
	WindowsSettings m_LastWindowsSettings;

	// Fractal Data
	FractalState m_FractalState;

	// Configuration Loading Data
	const std::filesystem::path m_DefaultConfigurationFilepath = "Internal/Configurations/Default.fractal";
	const std::filesystem::path m_PresetsFilepath = "Internal/Configurations/Presets/";

	std::filesystem::path m_CurrentConfigurationFilepath;
	std::filesystem::path m_ConfigurationFilepathToLoad;

	std::vector<std::filesystem::path> m_RecentConfigurationFilepaths;

	// Windows Vector
	std::vector<Scope<BaseWindow>> m_Windows;
};