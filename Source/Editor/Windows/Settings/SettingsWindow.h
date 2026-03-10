#pragma once

#include "Editor/BaseWindow.h"

#include "Core/Settings/Settings.h"

#include <vector>
#include <unordered_map>
#include <string>

class SettingsWindow : public BaseWindow {
public:
	SettingsWindow(bool& isOpen);

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnUIRender() override;
private:
	void DrawSettings();

	void DrawApplicationSettings(ApplicationSettings& applicationSettings);
	void DrawEditorSettings(EditorSettings& editorSettings);
	void DrawExportSettings(ExportSettings& exportSettings);
	void DrawNavigationSettings(NavigationSettings& navigationSettings);
	void DrawRenderingSettings(RenderingSettings& renderingSettings);

	void DrawWIP();
private:
	std::vector<Category> m_Categories;
	Category m_CurrentCategory = Category::Application;

	std::vector<EditorTheme> m_Themes;
	std::vector<RenderingEngine> m_RenderingEngines;
	std::vector<WindowMode> m_WindowModes;
	std::vector<ExportImageFormat> m_ExportImageFormats;
};