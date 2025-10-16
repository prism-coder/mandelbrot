#pragma once

#include "Editor/BaseWindow.h"

#include "Renderer/Texture.h"

#include <filesystem>
#include <unordered_map>
#include <functional>

class ProjectWindow : public BaseWindow {
public:
	using ConfigurationLoadCallback = std::function<void(const std::filesystem::path&)>;

	ProjectWindow(bool& isOpen);

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnUIRender() override;

	void SetConfigurationLoadCallback(const ConfigurationLoadCallback& callback) { m_ConfigurationLoadCallback = callback; }
private:
	void DrawFolderContent(const std::filesystem::path& path);

	void CreateConfiguration(const std::filesystem::path& directory);
private:
	const std::filesystem::path m_AssetsDirectory = "Assets";
	std::filesystem::path m_CurrentDirectory = m_AssetsDirectory;

	Ref<Texture2D> m_FolderIcon;
	Ref<Texture2D> m_FileIcon;
	Ref<Texture2D> m_ConfigurationIcon;

	float m_Padding = 16.0f;
	float m_ThumbnailSize = 96.0f;

	ConfigurationLoadCallback m_ConfigurationLoadCallback;
};