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

	void DrawApplicationSettings(ApplicationSettings& application);
	void DrawEditorSettings(EditorSettings& editor);
	void DrawRenderingSettings(RenderingSettings& rendering);

	void DrawWIP();
private:
	std::vector<Category> m_Categories;
	Category m_CurrentCategory = Category::Application;

	std::vector<EditorTheme> m_Themes;
	std::vector<RenderingEngine> m_RenderingEngines;
};