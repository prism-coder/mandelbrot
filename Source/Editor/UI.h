#pragma once

#include "Core/Core.h"

#include "Editor/Color.h"

#include "Renderer/Texture.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <functional>
#include <type_traits>
#include <filesystem>

class UI {
public:
	static void Init();
	static void Shutdown();

	static void Begin();
	static void End();
private:
	static void NewFrame();
	static void SetupStyle();
public:
	// Vector widgets with Label
	static bool Vec2(const char* label, glm::vec2& value, float resetValue = 0.0f);
	static bool Vec3(const char* label, glm::vec3& value, float resetValue = 0.0f);
	static bool Vec4(const char* label, glm::vec4& value, float resetValue = 0.0f);

	// Vector widgets without Label
	static bool Vec2(glm::vec2& value, float resetValue = 0.0f);
	static bool Vec3(glm::vec3& value, float resetValue = 0.0f);
	static bool Vec4(glm::vec4& value, float resetValue = 0.0f);

	// Color Edit widgets with Label
	static bool ColorEdit3(const char* label, glm::vec3& color);
	static bool ColorEdit4(const char* label, glm::vec4& color);

	// Color Edit widgets without Label
	static bool ColorEdit3(glm::vec3& color);
	static bool ColorEdit4(glm::vec4& color);

	// Color Picker widgets with Label
	static bool ColorPicker3(const char* label, glm::vec3& color);
	static bool ColorPicker4(const char* label, glm::vec4& color);

	// Color Picker widgets without Label
	static bool ColorPicker3(glm::vec3& color);
	static bool ColorPicker4(glm::vec4& color);

	// Float widgets with Label
	static bool DragFloat(const char* label, float& value, float min = 0.0f, float max = 0.0f, float speed = 0.01f);
	static bool SliderFloat(const char* label, float& value, float min = 0.0f, float max = 0.0f);
	
	// Float widgets without Label
	static bool DragFloat(float& value, float min = 0.0f, float max = 0.0f, float speed = 0.01f);
	static bool SliderFloat(float& value, float min = 0.0f, float max = 0.0f);

	// Int widgets with Label
	static bool DragInt(const char* label, int& value, int min = 0, int max = 0, float speed = 0.01f);
	static bool SliderInt(const char* label, int& value, int min = 0, int max = 0);

	// Int widgets without Label
	static bool DragInt(int& value, int min = 0, int max = 0, float speed = 0.01f);
	static bool SliderInt(int& value, int min = 0, int max = 0);

	// Slot widgets with Label
	static bool ConfigurationSlot(const char* label, std::filesystem::path& configurationFilepath);

	// Bool widget with Label
	static bool Bool(const char* label, bool& value);

	// Bool widget without Label
	static bool Bool(bool& value);

	// Text widgets with Label
	static void Text(const char* label, const std::string& text);
	static bool Textarea(const char* label, std::string& text, bool valid = true);
	static bool InputText(const char* label, std::string& text, bool valid = true);
	static void DisabledText(const char* label, const std::string& text);
	static void DisabledInputText(const char* label, std::string& text, bool valid = true);
	static void DisabledTextarea(const char* label, std::string& text, bool valid = true);
	static void Link(const char* label, const std::string& link);

	// Text widgets without Label
	static void Text(const std::string& text);
	static bool InputText(std::string& text, bool valid = true);
	static void DisabledText(const std::string& text);
	static void DisabledInputText(std::string& text, bool valid = true);

	// Spacing widgets
	static void Spacing();
	static void Header(const char* label);
	static void Separator();

	// Collapsing Header widget with Label
	static bool CollapsingHeader(const char* label, const std::function<void()>& popupItems = nullptr, ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_CollapsingHeader);
	
	// Collapsing Header widget without Label
	static bool CollapsingHeader(const std::function<void()>& popupItems = nullptr, ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_CollapsingHeader);
	
	static bool Button(const char* label, const glm::vec2& size = { 0.0f, 0.0f }, bool enabled = true);
	static bool RowButton(const char* label, bool enabled = true);
	static void Tooltip(const char* text);
	
	template<typename T, typename UIFunction>
	static bool List(const char* label, std::vector<T>& vector, UIFunction uiFunction);

	template<typename T>
	static bool Dropdown(const char* label, const std::vector<T>& options, T& selected, std::string(*itemToString)(const T&));
private:
	UI() = default;

	static void BeginPropertyGrid(const char* label);
	static void EndPropertyGrid();

	static bool VecComponent(const char* label, float& value, float resetValue, const ImVec4& buttonColor);

	static bool AssetSlot(const char* label, std::filesystem::path& assetFilepath, Ref<Texture2D> icon, const char* payloadID);

	static const char* HiddenLabel(const char* label);
private:
	inline static std::vector<std::string> s_LabelCache;

	inline static const char* s_EmptyLabel = "##";

	inline static Ref<Texture2D> s_EmptyImage = nullptr;
	inline static Ref<Texture2D> s_EllipsisIcon = nullptr;
	inline static Ref<Texture2D> s_ConfigurationIcon = nullptr;
};

template<typename T, typename UIFunction>
inline bool UI::List(const char* label, std::vector<T>& vector, UIFunction uiFunction) {
	BeginPropertyGrid(label);
	bool changed = false;

	if (UI::CollapsingHeader(label)) {
		ImGui::PushID(label);

		int itemToRemove = -1;

		for (int i = 0; i < vector.size(); ++i) {
			ImGui::PushID(i);

			if (ImGui::BeginTable("LIST_ITEM_TABLE", 2, ImGuiTableFlags_SizingStretchProp)) {
				ImGui::TableSetupColumn("ITEM", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("DELETE", ImGuiTableColumnFlags_WidthFixed, ImGui::GetFrameHeight());

				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::PushItemWidth(-1);

				uiFunction(vector[i]);

				ImGui::PopItemWidth();

				ImGui::TableSetColumnIndex(1);
				if (UI::Button("-", { -1, ImGui::GetFrameHeight() })) {
					itemToRemove = i;
				}
				UI::Tooltip("Remove");

				ImGui::EndTable();
			}

			ImGui::PopID();
		}

		if (itemToRemove != -1) {
			vector.erase(vector.begin() + itemToRemove);
			changed = true;
		}

		float availableWidth = ImGui::GetContentRegionAvail().x;

		if (UI::Button("Add (+)", { availableWidth, 0.0f })) {
			vector.emplace_back();
			changed = true;
		}

		ImGui::PopID();
	}

	EndPropertyGrid();

	return changed;
}

template<typename T>
inline bool UI::Dropdown(const char* label, const std::vector<T>& options, T& selected, std::string(*itemToString)(const T&)) {
	BeginPropertyGrid(label);

	bool changed = false;

	std::string previewValueString = itemToString(selected);
	const char* previewValue = previewValueString.c_str();

	if (ImGui::BeginCombo(HiddenLabel(label), previewValue)) {
		for (const auto& option : options) {
			const bool isSelected = (selected == option);

			std::string optionString = itemToString(option);
			if (ImGui::Selectable(optionString.c_str(), isSelected)) {
				selected = option;
				changed = true;
			}

			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	EndPropertyGrid();

	return changed;
}
