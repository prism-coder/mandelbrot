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
private:
	bool m_AboutWindowOpen = false;
	bool m_InspectorWindowOpen = true;
	bool m_ProjectWindowOpen = false;
	bool m_SettingsWindowOpen = false;
	bool m_StatisticsWindowOpen = false;
	bool m_ViewportWindowOpen = true;

	// Export Request
	bool m_RequestExport = false;

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