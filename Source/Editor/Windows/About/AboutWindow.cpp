#include "AboutWindow.h"

#include "Core/Log.h"
#include "Core/Application.h"
#include "Core/Settings/SettingsManager.h"

#include "Editor/UI.h"

AboutWindow::AboutWindow(bool& isOpen)
	: BaseWindow(isOpen)
{}

void AboutWindow::OnAttach() {
	Log::Trace("AboutWindow::OnAttach - Attaching About Window");
}

void AboutWindow::OnDetach() {
	Log::Trace("AboutWindow::OnDetach - Detaching About Window");
}

void AboutWindow::OnUpdate(Timestep ts) {}

void AboutWindow::OnUIRender() {
	if (!m_IsOpen) {
		return;
	}

	ImGui::Begin("About", &m_IsOpen);

	if (UI::CollapsingHeader("Application")) {
		const auto& settings = SettingsManager::Get().Application;
		std::string title = settings.Name;
		std::string version = settings.Version.GetName();
		std::string description = "A real-time 2D fractal explorer\nbuilt in C++ with OpenGL.";
		std::string author = "Prism | Software Developer";

		UI::DisabledInputText("Name", title);
		UI::DisabledInputText("Version", version);
		UI::DisabledInputText("Created by", author);
		UI::DisabledTextarea("Description", description);
		
		UI::Separator();
	}

	if (UI::CollapsingHeader("Links")) {
		UI::Link("TikTok", "https://www.tiktok.com/@prism.coder");
		UI::Link("YouTube", "https://www.youtube.com/@PrismCoder");
		UI::Link("Instagram", "https://www.instagram.com/prism.coder");
		UI::Link("Threads", "https://www.threads.com/@prism.coder");
		UI::Link("Twitter", "https://x.com/prism_coder");
		UI::Link("DEV.to", "https://dev.to/prismcoder");
		UI::Link("GitHub", "https://github.com/prism-coder");

		UI::Separator();

		UI::Link("Repository", "https://github.com/prism-coder/mandelbrot");

		UI::Separator();
	}

	if (UI::CollapsingHeader("Acknowledgments")) {
		UI::Text("This project was made possible thanks to these amazing open-source libraries:");
		ImGui::BulletText("ImGui");
		ImGui::BulletText("GLFW");
		ImGui::BulletText("GLAD");
		ImGui::BulletText("GLM");
		ImGui::BulletText("stb_image");
		ImGui::BulletText("yaml-cpp");

		UI::Separator();
	}

	if (UI::CollapsingHeader("License")) {
		UI::Text("Mandelbrot++ is distributed under the MIT License.");

		UI::Separator();
	}

	ImGui::End();
}
