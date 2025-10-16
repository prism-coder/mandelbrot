#include "ProjectWindow.h"

#include "Core/Log.h"

#include "Editor/Payload.h"

#include "Layers/Mandelbrot/Mandelbrot.h"
#include "Layers/Mandelbrot/MandelbrotSerializer.h"

ProjectWindow::ProjectWindow(bool& isOpen)
	: BaseWindow(isOpen)
{}

void ProjectWindow::OnAttach() {
	Log::Trace("ProjectWindow::OnAttach - Attaching Project Window");

	m_FolderIcon = Texture2D::Create("Internal/Icons/Folder.png");
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

	ImGui::BeginChild("FOLDER_CONTENT_CHILD", { 0, 0 }, ImGuiChildFlags_Borders);
	DrawFolderContent(m_AssetsDirectory);
	ImGui::EndChild();

	if (ImGui::BeginPopupContextItem(nullptr, ImGuiPopupFlags_MouseButtonRight)) {
		if (ImGui::BeginMenu("Create")) {
			if (ImGui::MenuItem("Configuration")) {
				CreateConfiguration(m_CurrentDirectory);
			}

			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}

	ImGui::End();
}

void ProjectWindow::DrawFolderContent(const std::filesystem::path& path) {
	{ // Breadcrumbs
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
	} // Breadcrumbs

	ImGui::Separator();

	float cellSize = m_ThumbnailSize + m_Padding;
	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);

	if (columnCount < 1) {
		columnCount = 1;
	}

	ImGui::Columns(columnCount, 0, false);

	for (const auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
		const auto& path = directoryEntry.path();
		const auto& filenameString = path.filename().string();
		const auto& extension = path.extension().string();
		const auto& pathString = path.string();

		Ref<Texture2D> icon;

		if (directoryEntry.is_directory()) {
			icon = m_FolderIcon;
		} else if (extension == ".fractal") {
			icon = m_ConfigurationIcon;
		} else {
			icon = m_FileIcon;
		}

		ImGui::PushID(filenameString.c_str());
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton(
			filenameString.c_str(),
			(ImTextureID)icon->GetHandle(),
			{ m_ThumbnailSize, m_ThumbnailSize },
			{ 0, 1 }, { 1, 0 },
			{ 0, 0, 0, 0 }
		);
		ImGui::PopStyleColor();

		if (extension == ".fractal") {
			if (ImGui::BeginDragDropSource()) {
				const char* itemPath = pathString.c_str();
				ImGui::SetDragDropPayload(Payload::Configuration, itemPath, strlen(itemPath) + 1);

				ImGui::Image((ImTextureID)icon->GetHandle(), { 64, 64 }, { 0, 1 }, { 1, 0 });
				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Text(filenameString.c_str());
				ImGui::Text("Configuration");
				ImGui::EndGroup();

				ImGui::EndDragDropSource();
			}
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			if (directoryEntry.is_directory()) {
				m_CurrentDirectory /= path.filename();
			} else if (extension == ".fractal" && m_ConfigurationLoadCallback) {
				m_ConfigurationLoadCallback(path);
			}
		}

		ImGui::TextWrapped(filenameString.c_str());
		ImGui::NextColumn();
		ImGui::PopID();
	}

	ImGui::Columns(1);
}

void ProjectWindow::CreateConfiguration(const std::filesystem::path& directory) {
	Mandelbrot mandelbrot;

	std::filesystem::path filepath = directory / "New Configuration.fractal";
	int i = 1;
	while (std::filesystem::exists(filepath)) {
		filepath = directory / ("New Configuration " + std::to_string(i++) + ".fractal");
	}

	MandelbrotSerializer serializer(mandelbrot);

	if (serializer.Serialize(filepath)) {
		Log::Info("ProjectWindow::CreateConfiguration - Created new Configuration at: " + filepath.string());
	} else {
		Log::Error("ProjectWindow::CreateConfiguration - Couldn't create Configuration at: " + filepath.string());
	}
}
