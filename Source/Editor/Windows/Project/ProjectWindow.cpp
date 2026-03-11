#include "ProjectWindow.h"

#include "Core/Log.h"

#include "Editor/Payload.h"

#include "Layers/Mandelbrot/Mandelbrot.h"
#include "Layers/Mandelbrot/MandelbrotSerializer.h"

#include <algorithm>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <shellapi.h>
#endif

ProjectWindow::ProjectWindow(bool& isOpen)
	: BaseWindow(isOpen)
{}

void ProjectWindow::OnAttach() {
	Log::Trace("ProjectWindow::OnAttach - Attaching Project Window");

	m_FullFolderIcon = Texture2D::Create("Internal/Icons/Folder-Full.png");
	m_EmptyFolderIcon = Texture2D::Create("Internal/Icons/Folder-Empty.png");
	m_FileIcon = Texture2D::Create("Internal/Icons/File.png");
	m_ConfigurationIcon = Texture2D::Create("Internal/Icons/Configuration.png");
}

void ProjectWindow::OnDetach() {
	Log::Trace("ProjectWindow::OnDetach - Detaching Project Window");
}

void ProjectWindow::OnUpdate(Timestep ts) {}

void ProjectWindow::OnUIRender() {
	if (!m_IsOpen) {
		return;
	}

	ImGui::Begin("Project", &m_IsOpen);

	DrawBreadcrumbs();
	ImGui::Separator();

	ImGui::BeginChild("FOLDER_CONTENT_CHILD", { 0, 0 }, ImGuiChildFlags_Borders);
	DrawFolderContent();
	DrawBackgroundContextMenu();
	ImGui::EndChild();

	if (m_OpenDeleteModal) {
		ImGui::OpenPopup("Delete##confirm");
		m_OpenDeleteModal = false;
	}

	DrawDeleteModal();

	ImGui::End();
}

void ProjectWindow::DrawFolderContent() {
	float cellSize   = m_ThumbnailSize + m_Padding;
	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount  = std::max(1, (int)(panelWidth / cellSize));

	ImGui::Columns(columnCount, nullptr, false);

	std::vector<std::filesystem::directory_entry> entries;
	for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
		entries.push_back(entry);
	}

	std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
		if (a.is_directory() != b.is_directory()) {
			return a.is_directory() > b.is_directory();
		}

		return a.path().filename().string() < b.path().filename().string();
	});

	for (const auto& entry : entries) {
		DrawItem(entry);
		ImGui::NextColumn();
	}

	ImGui::Columns(1);
}

void ProjectWindow::DrawItem(const std::filesystem::directory_entry& entry) {
	const auto& path          = entry.path();
	const std::string pathStr = path.string();
	const std::string name    = path.filename().string();
	const std::string ext     = path.extension().string();

	Ref<Texture2D> icon = GetIconForEntry(entry);

	ImGui::PushID(pathStr.c_str());

	bool isSelected = m_SelectedPath && *m_SelectedPath == path;
	ImGui::PushStyleColor(ImGuiCol_Button, isSelected
		? ImGui::GetStyleColorVec4(ImGuiCol_Header)
		: ImVec4(0, 0, 0, 0));

	ImGui::ImageButton(name.c_str(), (ImTextureID)icon->GetHandle(),
		{ m_ThumbnailSize, m_ThumbnailSize }, { 0, 1 }, { 1, 0 }, { 0, 0, 0, 0 });

	ImGui::PopStyleColor();

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		m_SelectedPath = path;
	}

	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
		if (entry.is_directory()) {
			m_CurrentDirectory /= path.filename();
			m_SelectedPath.reset();
		} else if (ext == ".fractal" && m_ConfigurationLoadCallback) {
			m_ConfigurationLoadCallback(path);
		}
	}

	HandleDragDropSource(entry);
	if (entry.is_directory()) {
		HandleDragDropTarget(path);
	}

	DrawItemContextMenu(path);
	DrawItemLabel(path);

	ImGui::PopID();
}

void ProjectWindow::DrawItemLabel(const std::filesystem::path& path) {
	if (m_RenamingPath && *m_RenamingPath == path) {
		ImGui::SetNextItemWidth(m_ThumbnailSize);

		if (m_FocusRenameInput) {
			ImGui::SetKeyboardFocusHere();
			m_FocusRenameInput = false;
		}

		bool confirmed  = ImGui::InputText("##rename", m_RenameBuffer, sizeof(m_RenameBuffer),
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);
		bool escaped    = ImGui::IsKeyPressed(ImGuiKey_Escape);
		bool deactivated = ImGui::IsItemDeactivated();

		if (escaped) {
			m_RenamingPath.reset();
		} else if (confirmed || deactivated) {
			RenameItem(path, m_RenameBuffer);
			m_RenamingPath.reset();
		}
	} else {
		ImGui::TextWrapped("%s", path.filename().string().c_str());
	}
}

void ProjectWindow::DrawBreadcrumbs() {
	std::filesystem::path breadcrumbPath;

	if (ImGui::Button(m_AssetsDirectory.filename().string().c_str())) {
		m_CurrentDirectory = m_AssetsDirectory;
	}

	auto relativePath = std::filesystem::relative(m_CurrentDirectory, m_AssetsDirectory);

	int partIndex = 0;
	for (const auto& part : relativePath) {
		if (part.empty() || part.string() == ".") {
			continue;
		}

		breadcrumbPath /= part;

		ImGui::SameLine();
		ImGui::Text(">");
		ImGui::SameLine();

		ImGui::PushID((part.string() + std::to_string(partIndex)).c_str());

		if (ImGui::Button(part.string().c_str())) {
			m_CurrentDirectory = m_AssetsDirectory / breadcrumbPath;
			ImGui::PopID();
			break;
		}

		ImGui::PopID();

		partIndex++;
	}
}

void ProjectWindow::DrawBackgroundContextMenu() {
	if (!ImGui::BeginPopupContextWindow("##bg_ctx",
		ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
	{
		return;
	}

	if (ImGui::BeginMenu("New")) {
		if (ImGui::MenuItem("Folder")) {
			CreateFolder(m_CurrentDirectory);
		}

		if (ImGui::MenuItem("Configuration")) {
			CreateConfiguration(m_CurrentDirectory);
		}

		ImGui::EndMenu();
	}

	ImGui::Separator();

	if (ImGui::MenuItem("Open in Explorer")) {
		OpenInExplorer(m_CurrentDirectory);
	}

	ImGui::EndPopup();
}

void ProjectWindow::DrawItemContextMenu(const std::filesystem::path& path) {
	if (!ImGui::BeginPopupContextItem()) {
		return;
	}

	m_SelectedPath = path;

	const bool isDir = std::filesystem::is_directory(path);
	const std::string ext = path.extension().string();

	if (isDir) {
		if (ImGui::MenuItem("Open")) {
			m_CurrentDirectory /= path.filename();
		}

		if (ImGui::BeginMenu("New")) {
			if (ImGui::MenuItem("Folder")) {
				CreateFolder(path);
			}

			if (ImGui::MenuItem("Configuration")) {
				CreateConfiguration(path);
			}

			ImGui::EndMenu();
		}

		ImGui::Separator();
		if (ImGui::MenuItem("Open in Explorer")) OpenInExplorer(path);
		ImGui::Separator();
	} else if (ext == ".fractal") {
		if (ImGui::MenuItem("Open") && m_ConfigurationLoadCallback) {
			m_ConfigurationLoadCallback(path);
		}

		if (ImGui::MenuItem("Duplicate")) {
			DuplicateItem(path);
		}

		ImGui::Separator();
	}

	if (ImGui::MenuItem("Rename")) {
		BeginRenaming(path, isDir ? path.filename().string() : path.stem().string());
	}

	if (ImGui::MenuItem("Delete")) {
		m_PendingDeletePath = path;
		m_OpenDeleteModal   = true;
	}

	ImGui::EndPopup();
}

void ProjectWindow::DrawDeleteModal() {
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, { 0.5f, 0.5f });

	if (!ImGui::BeginPopupModal("Delete##confirm", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		m_PendingDeletePath.reset(); // closed externally (e.g. Escape)
		return;
	}

	if (m_PendingDeletePath) {
		ImGui::Text("Are you sure you want to delete:");
		ImGui::TextUnformatted(m_PendingDeletePath->filename().string().c_str());

		if (std::filesystem::is_directory(*m_PendingDeletePath)) {
			ImGui::TextColored({ 1.0f, 0.4f, 0.4f, 1.0f },
				"This will delete the folder and ALL its contents.");
		}
	}

	ImGui::Separator();

	if (ImGui::Button("Delete", { 120, 0 })) {
		if (m_PendingDeletePath) DeleteItem(*m_PendingDeletePath);
		m_PendingDeletePath.reset();
		ImGui::CloseCurrentPopup();
	}

	ImGui::SameLine();

	if (ImGui::Button("Cancel", { 120, 0 })) {
		m_PendingDeletePath.reset();
		ImGui::CloseCurrentPopup();
	}

	ImGui::EndPopup();
}

void ProjectWindow::CreateFolder(const std::filesystem::path& directory) {
	std::filesystem::path folderPath = GenerateUniqueFilename(directory / "New Folder");
	std::error_code ec;

	if (std::filesystem::create_directory(folderPath, ec)) {
		Log::Info("ProjectWindow::CreateFolder - Created: " + folderPath.string());
		BeginRenaming(folderPath, folderPath.filename().string());
	} else {
		Log::Error("ProjectWindow::CreateFolder - Failed: " + folderPath.string() + " (" + ec.message() + ")");
	}
}

void ProjectWindow::CreateConfiguration(const std::filesystem::path& directory) {
	std::filesystem::path filepath = GenerateUniqueFilename(directory / "New Configuration.fractal");
	Mandelbrot mbrot;
	MandelbrotSerializer serializer(mbrot);

	if (serializer.Serialize(filepath)) {
		Log::Info("ProjectWindow::CreateConfiguration - Created: " + filepath.string());
		BeginRenaming(filepath, filepath.stem().string());
	} else {
		Log::Error("ProjectWindow::CreateConfiguration - Failed: " + filepath.string());
	}
}

void ProjectWindow::DeleteItem(const std::filesystem::path& path) {
	std::error_code ec;
	std::filesystem::remove_all(path, ec);

	if (ec) {
		Log::Error("ProjectWindow::DeleteItem - Failed: " + path.string() + " (" + ec.message() + ")");
		return;
	}

	Log::Info("ProjectWindow::DeleteItem - Deleted: " + path.string());

	if (m_SelectedPath && IsAncestorOf(path, *m_SelectedPath)) {
		m_SelectedPath.reset();
	}

	if (IsAncestorOf(path, m_CurrentDirectory)) {
		m_CurrentDirectory = m_AssetsDirectory;
	}
}

void ProjectWindow::RenameItem(const std::filesystem::path& oldPath, const std::string& newStem) {
	if (!IsValidFilename(newStem)) {
		Log::Warning("ProjectWindow::RenameItem - Invalid filename: " + newStem);
		return;
	}

	const bool isDir = std::filesystem::is_directory(oldPath);
	std::filesystem::path newPath = isDir
		? oldPath.parent_path() / newStem
		: oldPath.parent_path() / (newStem + oldPath.extension().string());

	if (newPath == oldPath) {
		return;
	}

	if (!IsInsideAssetsDirectory(newPath)) {
		Log::Warning("ProjectWindow::RenameItem - Target is outside Assets directory");
		return;
	}

	if (std::filesystem::exists(newPath)) {
		Log::Warning("ProjectWindow::RenameItem - Target already exists: " + newPath.string());
		return;
	}

	std::error_code ec;
	std::filesystem::rename(oldPath, newPath, ec);

	if (ec) {
		Log::Error("ProjectWindow::RenameItem - Failed: " + oldPath.string() + " (" + ec.message() + ")");
		return;
	}

	Log::Info("ProjectWindow::RenameItem - Renamed: " + oldPath.filename().string() + " -> " + newPath.filename().string());

	if (m_SelectedPath && IsAncestorOf(oldPath, *m_SelectedPath)) {
		auto rel = std::filesystem::relative(*m_SelectedPath, oldPath);
		m_SelectedPath = newPath / rel;
	}

	if (IsAncestorOf(oldPath, m_CurrentDirectory)) {
		auto rel = std::filesystem::relative(m_CurrentDirectory, oldPath);
		m_CurrentDirectory = newPath / rel;
	}
}

void ProjectWindow::DuplicateItem(const std::filesystem::path& path) {
	std::filesystem::path preferred = path.parent_path()
		/ (path.stem().string() + " Copy" + path.extension().string());
	std::filesystem::path dest = GenerateUniqueFilename(preferred);

	std::error_code ec;
	std::filesystem::copy(path, dest, std::filesystem::copy_options::recursive, ec);

	if (ec) {
		Log::Error("ProjectWindow::DuplicateItem - Failed: " + path.string() + " (" + ec.message() + ")");
	} else {
		Log::Info("ProjectWindow::DuplicateItem - Duplicated: " + path.filename().string() + " -> " + dest.filename().string());
	}
}

void ProjectWindow::MoveItem(const std::filesystem::path& source, const std::filesystem::path& targetDir) {
	std::filesystem::path dest = targetDir / source.filename();

	if (!IsInsideAssetsDirectory(dest)) {
		Log::Warning("ProjectWindow::MoveItem - Destination is outside Assets directory");
		return;
	}

	if (IsAncestorOf(source, targetDir)) {
		Log::Warning("ProjectWindow::MoveItem - Cannot move a folder into itself");
		return;
	}

	if (std::filesystem::exists(dest)) {
		Log::Warning("ProjectWindow::MoveItem - Destination already exists: " + dest.string());
		return;
	}

	std::error_code ec;
	std::filesystem::rename(source, dest, ec);

	if (ec) {
		Log::Error("ProjectWindow::MoveItem - Failed: " + source.string() + " (" + ec.message() + ")");
		return;
	}

	Log::Info("ProjectWindow::MoveItem - Moved: " + source.filename().string()
		+ " into " + dest.parent_path().filename().string());

	if (m_SelectedPath && IsAncestorOf(source, *m_SelectedPath)) {
		auto rel = std::filesystem::relative(*m_SelectedPath, source);
		m_SelectedPath = dest / rel;
	}

	if (IsAncestorOf(source, m_CurrentDirectory)) {
		auto rel = std::filesystem::relative(m_CurrentDirectory, source);
		m_CurrentDirectory = dest / rel;
	}
}

void ProjectWindow::OpenInExplorer(const std::filesystem::path& path) {
#ifdef _WIN32
	ShellExecuteW(nullptr, L"explore", path.wstring().c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
#endif
}

void ProjectWindow::HandleDragDropSource(const std::filesystem::directory_entry& entry) {
	if (!ImGui::BeginDragDropSource()) {
		return;
	}

	const auto& path      = entry.path();
	const std::string str = path.string();
	const bool isDir      = entry.is_directory();
	const std::string ext = path.extension().string();

	ImGui::SetDragDropPayload(isDir ? Payload::Folder : Payload::Configuration,
		str.c_str(), str.size() + 1);

	ImGui::Image((ImTextureID)GetIconForEntry(entry)->GetHandle(), { 64, 64 }, { 0, 1 }, { 1, 0 });
	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::TextUnformatted(path.filename().string().c_str());
	ImGui::TextUnformatted(isDir ? "Folder" : (ext == ".fractal" ? "Configuration" : "File"));
	ImGui::EndGroup();

	ImGui::EndDragDropSource();
}

void ProjectWindow::HandleDragDropTarget(const std::filesystem::path& targetDir) {
	if (!ImGui::BeginDragDropTarget()) {
		return;
	}

	auto tryAccept = [&](const char* payloadType) {
		if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload(payloadType)) {
			MoveItem(std::filesystem::path(std::string((const char*)p->Data, p->DataSize - 1)), targetDir);
		}
	};

	tryAccept(Payload::Configuration);
	tryAccept(Payload::Folder);

	ImGui::EndDragDropTarget();
}

Ref<Texture2D> ProjectWindow::GetIconForEntry(const std::filesystem::directory_entry& entry) const {
	if (entry.is_directory()) {
		if (std::filesystem::is_empty(entry.path())) {
			return m_EmptyFolderIcon;
		}

		return m_FullFolderIcon;
	}

	if (entry.path().extension().string() == ".fractal") {
		return m_ConfigurationIcon;
	}

	return m_FileIcon;
}

std::filesystem::path ProjectWindow::GenerateUniqueFilename(const std::filesystem::path& preferred) const {
	if (!std::filesystem::exists(preferred)) {
		return preferred;
	}

	const std::string stem   = preferred.stem().string();
	const std::string ext    = preferred.extension().string();
	const auto&       parent = preferred.parent_path();

	int i = 2;
	std::filesystem::path candidate;
	do {
		candidate = parent / (stem + " " + std::to_string(i++) + ext);
	} while (std::filesystem::exists(candidate));

	return candidate;
}

bool ProjectWindow::IsInsideAssetsDirectory(const std::filesystem::path& path) const {
	return IsAncestorOf(m_AssetsDirectory, path);
}

bool ProjectWindow::IsAncestorOf(const std::filesystem::path& ancestor, const std::filesystem::path& descendant) const {
	auto ancestorCanon = std::filesystem::weakly_canonical(ancestor);
	auto descCanon     = std::filesystem::weakly_canonical(descendant);
	auto result        = std::mismatch(ancestorCanon.begin(), ancestorCanon.end(),
	                                   descCanon.begin(),     descCanon.end());
	return result.first == ancestorCanon.end();
}

void ProjectWindow::BeginRenaming(const std::filesystem::path& path, const std::string& initialText) {
	m_RenamingPath     = path;
	m_FocusRenameInput = true;
	snprintf(m_RenameBuffer, sizeof(m_RenameBuffer), "%s", initialText.c_str());
}

bool ProjectWindow::IsValidFilename(const std::string& name) {
	if (name.empty() || name == "." || name == "..") {
		return false;
	}

	for (char c : name) {
		if (c == '/' || c == '\\' || c == ':' || c == '*' ||
			c == '?' || c == '"'  || c == '<' || c == '>' || c == '|')
		{
			return false;
		}
	}

	return true;
}
