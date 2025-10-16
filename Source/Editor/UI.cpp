#include "UI.h"

#include "Core/Log.h"
#include "Core/Application.h"
#include "Core/Settings/SettingsManager.h"

#include "Editor/Payload.h"
#include "Editor/Theme/ThemeManager.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void UI::Init() {
	Log::Trace("UI::Init - Initializing the UI Library");

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

	const auto& settings = SettingsManager::Get().Editor;

	if (ThemeManager::Load(ThemeManager::Select(settings.Appearance.Theme))) {
		ThemeManager::Apply(ThemeManager::Get());
	} else {
		ImGui::StyleColorsDark();
	}

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	style.FontSizeBase = (float)settings.Appearance.FontSize;

	// Setup Platform/Renderer backends
	GLFWwindow* window = Application::Get().GetWindowHandle();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	TextureSpecification textureSpecification;
	s_EmptyImage = Texture2D::Create(textureSpecification);
	s_EllipsisIcon = Texture2D::Create("Internal/Icons/Ellipsis.png");
	s_ConfigurationIcon = Texture2D::Create("Internal/Icons/Configuration.png");
}

void UI::Shutdown() {
	Log::Trace("UI::Shutdown - Shutting down the UI Library");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	s_LabelCache.clear();
}

void UI::Begin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	NewFrame();

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
}

void UI::End() {
	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::Get();
	io.DisplaySize = ImVec2((float)app.GetWidth(), (float)app.GetHeight());

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void UI::SetupStyle() {
	// Excellency style by gonzaloivan121 from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(10.0f, 10.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 6.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 6.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(8.0f, 6.0f);
	style.FrameRounding = 6.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(6.0f, 6.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 11.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 6.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 6.0f;
	style.TabRounding = 6.0f;
	style.TabBorderSize = 1.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.501960813999176f, 0.501960813999176f, 0.501960813999176f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08235294371843338f, 0.08235294371843338f, 0.08235294371843338f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.196078434586525f, 0.196078434586525f, 0.196078434586525f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.1019607856869698f, 0.1019607856869698f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.09019608050584793f, 0.09019608050584793f, 0.09019608050584793f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08235294371843338f, 0.08235294371843338f, 0.08235294371843338f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08235294371843338f, 0.08235294371843338f, 0.08235294371843338f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1529411822557449f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4117647111415863f, 0.4117647111415863f, 0.4117647111415863f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.7529411911964417f, 0.7529411911964417f, 0.7529411911964417f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 0.699999988079071f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.658823549747467f, 0.658823549747467f, 0.658823549747467f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.2196078449487686f, 0.2196078449487686f, 0.2196078449487686f, 0.7839999794960022f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2745098173618317f, 0.2745098173618317f, 0.2745098173618317f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2196078449487686f, 0.2196078449487686f, 0.2196078449487686f, 0.5879999995231628f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.1843137294054031f, 0.1843137294054031f, 0.1843137294054031f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1843137294054031f, 0.1843137294054031f, 0.1843137294054031f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.1843137294054031f, 0.1843137294054031f, 0.1843137294054031f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.1019607856869698f, 0.1019607856869698f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1529411822557449f, 0.7254902124404907f, 0.9490196108818054f, 0.5879999995231628f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1529411822557449f, 0.7254902124404907f, 0.9490196108818054f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.9098039269447327f, 0.9098039269447327f, 0.9098039269447327f, 0.25f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.8117647171020508f, 0.8117647171020508f, 0.8117647171020508f, 0.6700000166893005f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4588235318660736f, 0.4588235318660736f, 0.4588235318660736f, 0.949999988079071f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.08235294371843338f, 0.08235294371843338f, 0.08235294371843338f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(1.0f, 0.8823529481887817f, 0.529411792755127f, 0.1180000007152557f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(1.0f, 0.8823529481887817f, 0.529411792755127f, 0.2349999994039536f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08235294371843338f, 0.08235294371843338f, 0.08235294371843338f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(1.0f, 0.8823529481887817f, 0.529411792755127f, 0.1180000007152557f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6117647290229797f, 0.6117647290229797f, 0.6117647290229797f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4313725531101227f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.9019607901573181f, 0.7019608020782471f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1843137294054031f, 0.1843137294054031f, 0.1843137294054031f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.1019607856869698f, 0.1019607856869698f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.1529411822557449f, 0.7254902124404907f, 0.9490196108818054f, 0.3499999940395355f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.1529411822557449f, 0.7254902124404907f, 0.9490196108818054f, 0.800000011920929f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}

void UI::NewFrame() {
	s_LabelCache.clear();
}

bool UI::Vec2(const char* label, glm::vec2& value, float resetValue) {
	BeginPropertyGrid(label);
	ImGui::BeginTable(label, 2);

	ImGui::TableNextColumn(); bool changedX = VecComponent("X", value.x, resetValue, Color::Red);
	ImGui::TableNextColumn(); bool changedY = VecComponent("Y", value.y, resetValue, Color::Green);

	ImGui::EndTable();
	EndPropertyGrid();

	return changedX || changedY;
}

bool UI::Vec3(const char* label, glm::vec3& value, float resetValue) {
	BeginPropertyGrid(label);
	ImGui::BeginTable(label, 3);

	ImGui::TableNextColumn(); bool changedX = VecComponent("X", value.x, resetValue, Color::Red);
	ImGui::TableNextColumn(); bool changedY = VecComponent("Y", value.y, resetValue, Color::Green);
	ImGui::TableNextColumn(); bool changedZ = VecComponent("Z", value.z, resetValue, Color::Blue);

	ImGui::EndTable();
	EndPropertyGrid();

	return changedX || changedY || changedZ;
}

bool UI::Vec4(const char* label, glm::vec4& value, float resetValue) {
	BeginPropertyGrid(label);
	ImGui::BeginTable(label, 4);

	ImGui::TableNextColumn(); bool changedX = VecComponent("X", value.x, resetValue, Color::Red);
	ImGui::TableNextColumn(); bool changedY = VecComponent("Y", value.y, resetValue, Color::Green);
	ImGui::TableNextColumn(); bool changedZ = VecComponent("Z", value.z, resetValue, Color::Blue);
	ImGui::TableNextColumn(); bool changedW = VecComponent("W", value.w, resetValue, Color::Orange);

	ImGui::EndTable();
	EndPropertyGrid();

	return changedX || changedY || changedZ || changedW;
}

bool UI::Vec2(glm::vec2& value, float resetValue) {
	ImGui::BeginTable("##", 2);

	ImGui::TableNextColumn(); bool changedX = VecComponent("X", value.x, resetValue, Color::Red);
	ImGui::TableNextColumn(); bool changedY = VecComponent("Y", value.y, resetValue, Color::Green);

	ImGui::EndTable();

	return changedX || changedY;
}

bool UI::Vec3(glm::vec3& value, float resetValue) {
	ImGui::BeginTable("##", 3);

	ImGui::TableNextColumn(); bool changedX = VecComponent("X", value.x, resetValue, Color::Red);
	ImGui::TableNextColumn(); bool changedY = VecComponent("Y", value.y, resetValue, Color::Green);
	ImGui::TableNextColumn(); bool changedZ = VecComponent("Z", value.z, resetValue, Color::Blue);

	ImGui::EndTable();

	return changedX || changedY || changedZ;
}

bool UI::Vec4(glm::vec4& value, float resetValue) {
	ImGui::BeginTable("##", 4);

	ImGui::TableNextColumn(); bool changedX = VecComponent("X", value.x, resetValue, Color::Red);
	ImGui::TableNextColumn(); bool changedY = VecComponent("Y", value.y, resetValue, Color::Green);
	ImGui::TableNextColumn(); bool changedZ = VecComponent("Z", value.z, resetValue, Color::Blue);
	ImGui::TableNextColumn(); bool changedW = VecComponent("W", value.w, resetValue, Color::Orange);

	ImGui::EndTable();

	return changedX || changedY || changedZ || changedW;
}

bool UI::ColorEdit3(const char* label, glm::vec3& color) {
	BeginPropertyGrid(label);
	bool changed = ImGui::ColorEdit3(HiddenLabel(label), glm::value_ptr(color));
	EndPropertyGrid();
	return changed;
}

bool UI::ColorEdit4(const char* label, glm::vec4& color) {
	BeginPropertyGrid(label);
	bool changed = ImGui::ColorEdit4(HiddenLabel(label), glm::value_ptr(color));
	EndPropertyGrid();
	return changed;
}

bool UI::ColorEdit3(glm::vec3& color) {
	return ImGui::ColorEdit3(s_EmptyLabel, glm::value_ptr(color));
}

bool UI::ColorEdit4(glm::vec4& color) {
	return ImGui::ColorEdit4(s_EmptyLabel, glm::value_ptr(color));
}

bool UI::ColorPicker3(const char* label, glm::vec3& color) {
	BeginPropertyGrid(label);
	bool changed = ImGui::ColorPicker3(HiddenLabel(label), glm::value_ptr(color));
	EndPropertyGrid();
	return changed;
}

bool UI::ColorPicker4(const char* label, glm::vec4& color) {
	BeginPropertyGrid(label);
	bool changed = ImGui::ColorPicker4(HiddenLabel(label), glm::value_ptr(color));
	EndPropertyGrid();
	return changed;
}

bool UI::ColorPicker3(glm::vec3& color) {
	return ImGui::ColorPicker3(s_EmptyLabel, glm::value_ptr(color));
}

bool UI::ColorPicker4(glm::vec4& color) {
	return ImGui::ColorPicker4(s_EmptyLabel, glm::value_ptr(color));
}

bool UI::DragFloat(const char* label, float& value, float min, float max, float speed) {
	BeginPropertyGrid(label);
	bool changed = ImGui::DragFloat(HiddenLabel(label), &value, speed, min, max);
	EndPropertyGrid();
	return changed;
}

bool UI::SliderFloat(const char* label, float& value, float min, float max) {
	BeginPropertyGrid(label);
	bool changed = ImGui::SliderFloat(HiddenLabel(label), &value, min, max);
	EndPropertyGrid();
	return changed;
}

bool UI::DragFloat(float& value, float min, float max, float speed) {
	return ImGui::DragFloat(s_EmptyLabel, &value, speed, min, max);
}

bool UI::SliderFloat(float& value, float min, float max) {
	return ImGui::SliderFloat(s_EmptyLabel, &value, min, max);
}

bool UI::DragInt(const char* label, int& value, int min, int max, float speed) {
	BeginPropertyGrid(label);
	bool changed = ImGui::DragInt(HiddenLabel(label), &value, speed, min, max);
	EndPropertyGrid();
	return changed;
}

bool UI::SliderInt(const char* label, int& value, int min, int max) {
	BeginPropertyGrid(label);
	bool changed = ImGui::SliderInt(HiddenLabel(label), &value, min, max);
	EndPropertyGrid();
	return changed;
}

bool UI::DragInt(int& value, int min, int max, float speed) {
	return ImGui::DragInt(s_EmptyLabel, &value, speed, min, max);
}

bool UI::SliderInt(int& value, int min, int max) {
	return ImGui::SliderInt(s_EmptyLabel, &value, min, max);
}

bool UI::ConfigurationSlot(const char* label, std::filesystem::path& configurationFilepath) {
	return AssetSlot(label, configurationFilepath, s_ConfigurationIcon, Payload::Configuration);
}

bool UI::Bool(const char* label, bool& value) {
	BeginPropertyGrid(label);
	bool changed = ImGui::Checkbox(HiddenLabel(label), &value);
	EndPropertyGrid();
	return changed;
}

bool UI::Bool(bool& value) {
	return ImGui::Checkbox(s_EmptyLabel, &value);
}

void UI::Text(const char* label, const std::string& text) {
	BeginPropertyGrid(label);
	ImGui::Text(text.c_str());
	EndPropertyGrid();
}

bool UI::Textarea(const char* label, std::string& text, bool valid) {
	BeginPropertyGrid(label);

	if (!valid) {
		ImGui::PushStyleColor(ImGuiCol_Border, Color::Red);
	}

	bool changed = ImGui::InputTextMultiline(HiddenLabel(label), &text);

	if (!valid) {
		ImGui::PopStyleColor();
	}

	EndPropertyGrid();

	return changed;
}

bool UI::InputText(const char* label, std::string& text, bool valid) {
	BeginPropertyGrid(label);

	if (!valid) {
		ImGui::PushStyleColor(ImGuiCol_Border, Color::Red);
	}

	bool changed = ImGui::InputText(HiddenLabel(label), &text);

	if (!valid) {
		ImGui::PopStyleColor();
	}

	EndPropertyGrid();

	return changed;
}

void UI::DisabledText(const char* label, const std::string& text) {
	BeginPropertyGrid(label);

	ImGui::BeginDisabled();
	ImGui::Text(text.c_str());
	ImGui::EndDisabled();

	EndPropertyGrid();
}

void UI::DisabledInputText(const char* label, std::string& text, bool valid) {
	BeginPropertyGrid(label);
	ImGui::BeginDisabled();

	if (!valid) {
		ImGui::PushStyleColor(ImGuiCol_Border, Color::Red);
	}

	ImGui::InputText(HiddenLabel(label), &text);

	if (!valid) {
		ImGui::PopStyleColor();
	}

	ImGui::EndDisabled();
	EndPropertyGrid();
}

void UI::DisabledTextarea(const char* label, std::string& text, bool valid) {
	BeginPropertyGrid(label);
	ImGui::BeginDisabled();

	if (!valid) {
		ImGui::PushStyleColor(ImGuiCol_Border, Color::Red);
	}

	ImGui::InputTextMultiline(HiddenLabel(label), &text, { 0, 0 }, ImGuiInputTextFlags_AllowTabInput);

	if (!valid) {
		ImGui::PopStyleColor();
	}

	ImGui::EndDisabled();
	EndPropertyGrid();
}

void UI::Link(const char* label, const std::string& link) {
	BeginPropertyGrid(label);

	ImGui::TextLinkOpenURL(link.c_str());

	EndPropertyGrid();
}

void UI::Text(const std::string& text) {
	ImGui::Text(text.c_str());
}

bool UI::InputText(std::string& text, bool valid) {
	if (!valid) {
		ImGui::PushStyleColor(ImGuiCol_Border, Color::Red);
	}

	bool changed = ImGui::InputText(s_EmptyLabel, &text);

	if (!valid) {
		ImGui::PopStyleColor();
	}

	return changed;
}

void UI::DisabledText(const std::string& text) {
	ImGui::BeginDisabled();
	ImGui::Text(text.c_str());
	ImGui::EndDisabled();
}

void UI::DisabledInputText(std::string& text, bool valid) {
	ImGui::BeginDisabled();

	if (!valid) {
		ImGui::PushStyleColor(ImGuiCol_Border, Color::Red);
	}

	ImGui::InputText(s_EmptyLabel, &text);

	if (!valid) {
		ImGui::PopStyleColor();
	}

	ImGui::EndDisabled();
}

void UI::Spacing() {
	ImGui::Spacing();
}

void UI::Header(const char* label) {
	Spacing();
	ImGui::SeparatorText(label);
	Spacing();
}

void UI::Separator() {
	ImGui::Separator();
}

bool UI::CollapsingHeader(const char* label, const std::function<void()>& popupItems, ImGuiTreeNodeFlags flags) {
	ImGui::PushID(label);

	flags	|= ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_AllowOverlap
			| ImGuiTreeNodeFlags_FramePadding;

	float lineHeightWithSpacing = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	float lineHeight = lineHeightWithSpacing * 0.5f;
	bool isOpen = ImGui::TreeNodeEx(label, flags);

	if (popupItems) {
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeightWithSpacing);

		if (ImGui::ImageButton(HiddenLabel(label), (ImTextureRef)s_EllipsisIcon->GetHandle(), { lineHeight, lineHeight })) {
			ImGui::OpenPopup("CollapsingHeaderPopup");
		}

		if (ImGui::BeginPopup("CollapsingHeaderPopup")) {
			popupItems();
			ImGui::EndPopup();
		}
	}

	ImGui::PopID();

	return isOpen;
}

bool UI::CollapsingHeader(const std::function<void()>& popupItems, ImGuiTreeNodeFlags flags) {
	ImGui::PushID(s_EmptyLabel);

	flags |= ImGuiTreeNodeFlags_DefaultOpen
		| ImGuiTreeNodeFlags_AllowOverlap
		| ImGuiTreeNodeFlags_FramePadding;

	float lineHeightWithSpacing = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	float lineHeight = lineHeightWithSpacing * 0.5f;
	bool isOpen = ImGui::TreeNodeEx(s_EmptyLabel, flags);

	if (popupItems) {
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeightWithSpacing);

		if (ImGui::ImageButton(s_EmptyLabel, (ImTextureRef)s_EllipsisIcon->GetHandle(), { lineHeight, lineHeight })) {
			ImGui::OpenPopup("CollapsingHeaderPopup");
		}

		if (ImGui::BeginPopup("CollapsingHeaderPopup")) {
			popupItems();
			ImGui::EndPopup();
		}
	}

	ImGui::PopID();

	return isOpen;
}

bool UI::Button(const char* label, const glm::vec2& size, bool enabled) {
	if (!enabled) {
		ImGui::BeginDisabled();
	}

	bool clicked = ImGui::Button(label, { size.x, size.y });

	if (!enabled) {
		ImGui::EndDisabled();
	}

	return clicked;
}

bool UI::RowButton(const char* label, bool enabled) {
	BeginPropertyGrid(label);

	if (!enabled) {
		ImGui::BeginDisabled();
	}

	bool clicked = ImGui::Button(label, {ImGui::GetContentRegionAvail().x, 0.0f});

	if (!enabled) {
		ImGui::EndDisabled();
	}

	EndPropertyGrid();

	return clicked;
}

void UI::Tooltip(const char* text) {
	ImGui::SetItemTooltip(text);
}

void UI::BeginPropertyGrid(const char* label) {
	const auto& settings = SettingsManager::Get().Editor.Appearance;

	ImGui::PushID(label);
	ImGui::BeginTable(label, 2, ImGuiTableFlags_BordersInnerV);
	ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, settings.ColumnWidth);
	ImGui::TableSetupColumn("Widget", ImGuiTableColumnFlags_WidthStretch);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted(label);
	ImGui::TableSetColumnIndex(1);
	ImGui::PushItemWidth(-1.0f);
}

void UI::EndPropertyGrid() {
	ImGui::EndTable();
	ImGui::PopID();
}

bool UI::VecComponent(const char* label, float& value, float resetValue, const ImVec4& buttonColor) {
	bool changed = false;

	ImGui::PushID(label);

	float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
	if (ImGui::Button(label, buttonSize)) {
		value = resetValue;
		changed = true;
	}
	ImGui::PopStyleColor();

	ImGui::SameLine();
	ImGui::PushItemWidth(-1.0f);
	
	changed = ImGui::DragFloat(HiddenLabel(label), &value, 0.1f, 0.0f, 0.0f, "%.3f");
	ImGui::PopItemWidth();

	ImGui::PopID();

	return changed;
}

bool UI::AssetSlot(const char* label, std::filesystem::path& assetFilepath, Ref<Texture2D> icon, const char* payloadID) {
	bool changed = false;
	BeginPropertyGrid(label);

	std::string assetName = "None";
	bool hasAsset = !assetFilepath.empty();

	if (hasAsset) {
		assetName = assetFilepath.filename().string();
	}

	ImGui::BeginDisabled();
	float lineHeight = (GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f) * 0.5f;
	ImVec2 iconSize = { lineHeight, lineHeight };
	ImGui::ImageButton(
		HiddenLabel((std::string(label) + "icon").c_str()),
		(ImTextureID)icon->GetHandle(),
		iconSize,
		{ 0, 1 }, { 1, 0 }
	);
	ImGui::SameLine();
	ImGui::InputText(HiddenLabel(label), &assetName);
	ImGui::EndDisabled();

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadID)) {
			const char* path = (const char*)payload->Data;
			assetFilepath = std::filesystem::path(path);
			changed = true;
		}
		ImGui::EndDragDropTarget();
	}

	if (hasAsset) {
		ImGui::SameLine();
		if (ImGui::Button(("x##" + std::string(label)).c_str())) {
			assetFilepath.clear();
			changed = true;
		}
	}

	EndPropertyGrid();
	return changed;
}

const char* UI::HiddenLabel(const char* label) {
	std::string hiddenLabel = "##" + std::string(label);
	s_LabelCache.push_back(hiddenLabel);

	return s_LabelCache.back().c_str();
}
