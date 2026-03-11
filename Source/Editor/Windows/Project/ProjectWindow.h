#pragma once

#include "Editor/BaseWindow.h"

#include "Renderer/Texture.h"

#include <filesystem>
#include <functional>
#include <optional>
#include <string>

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
	// Drawing
	void DrawFolderContent();
	void DrawBreadcrumbs();
	void DrawItem(const std::filesystem::directory_entry& entry);
	void DrawItemLabel(const std::filesystem::path& path);

	// Context menus & modals
	void DrawBackgroundContextMenu();
	void DrawItemContextMenu(const std::filesystem::path& path);
	void DrawDeleteModal();

	// File operations
	void CreateFolder(const std::filesystem::path& directory);
	void CreateConfiguration(const std::filesystem::path& directory);
	void DeleteItem(const std::filesystem::path& path);
	void RenameItem(const std::filesystem::path& oldPath, const std::string& newStem);
	void DuplicateItem(const std::filesystem::path& path);
	void MoveItem(const std::filesystem::path& source, const std::filesystem::path& targetDir);
	void OpenInExplorer(const std::filesystem::path& path);

	// Drag & Drop
	void HandleDragDropSource(const std::filesystem::directory_entry& entry);
	void HandleDragDropTarget(const std::filesystem::path& targetDir);

	// Helpers
	Ref<Texture2D> GetIconForEntry(const std::filesystem::directory_entry& entry) const;
	std::filesystem::path GenerateUniqueFilename(const std::filesystem::path& preferred) const;
	bool IsInsideAssetsDirectory(const std::filesystem::path& path) const;
	bool IsAncestorOf(const std::filesystem::path& ancestor, const std::filesystem::path& descendant) const;
	void BeginRenaming(const std::filesystem::path& path, const std::string& initialText);
	static bool IsValidFilename(const std::string& name);

private:
	const std::filesystem::path m_AssetsDirectory = "Assets";
	std::filesystem::path m_CurrentDirectory = m_AssetsDirectory;

	Ref<Texture2D> m_FullFolderIcon;
	Ref<Texture2D> m_EmptyFolderIcon;
	Ref<Texture2D> m_FileIcon;
	Ref<Texture2D> m_ConfigurationIcon;

	float m_Padding = 16.0f;
	float m_ThumbnailSize = 96.0f;

	// Rename state
	std::optional<std::filesystem::path> m_RenamingPath;
	char m_RenameBuffer[256] = {};
	bool m_FocusRenameInput = false;

	// Delete state
	std::optional<std::filesystem::path> m_PendingDeletePath;
	bool m_OpenDeleteModal = false;

	// Selection
	std::optional<std::filesystem::path> m_SelectedPath;

	ConfigurationLoadCallback m_ConfigurationLoadCallback;
};