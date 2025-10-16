#include "SettingsSerializer.h"

#include "Core/Log.h"

#include "Utilities/Utilities.h"

#include <fstream>

namespace YAML {
	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs) {
			if (!node.IsSequence() || node.size() != 2) return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	static Emitter& operator<<(Emitter& out, const glm::vec3& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << v.z << EndSeq;
		return out;
	}

	static Emitter& operator<<(Emitter& out, const glm::vec2& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << EndSeq;
		return out;
	}
}

SettingsSerializer::SettingsSerializer(Settings& settings)
	: m_Settings(settings)
{
	Log::Trace("SettingsSerializer::SettingsSerializer - Setting up Settings Serializer");
}

bool SettingsSerializer::Serialize(const std::filesystem::path& filepath) {
	Log::Trace("SettingsSerializer::Serialize - Saving Settings");

	if (filepath.empty()) {
		Log::Error("SettingsSerializer::Serialize - Filepath is empty");
		return false;
	}

	Log::Trace("SettingsSerializer::Serialize - Serializing Settings into file: " + filepath.string());

	YAML::Emitter out;
	{
		out << YAML::BeginMap; // Root
		out << YAML::Key << "Settings" << YAML::Value;
		{
			out << YAML::BeginMap; // Settings
			{
				SerializeApplicationSettings(out);
				SerializeEditorSettings(out);
				SerializeRenderingSettings(out);
			}
			out << YAML::EndMap; // Settings
		}
		out << YAML::EndMap; // Root
	}

	std::ofstream fout(filepath);

	if (fout.bad()) {
		Log::Error("SettingsSerializer::Serialize - Error during Settings Serialization: Bad file");
		return false;
	}

	if (!out.good()) {
		Log::Error("SettingsSerializer::Serialize - Error during Settings Serialization: Bad format");
		return false;
	}

	fout << out.c_str();

	Log::Trace("SettingsSerializer::Serialize - Settings Serialization complete");

	return true;
}

bool SettingsSerializer::Deserialize(const std::filesystem::path& filepath) {
	YAML::Node data;

	try {
		Log::Trace("SettingsSerializer::Deserialize - Attempting to read Settings file: " + filepath.string());
		data = YAML::LoadFile(filepath.string());
	} catch (YAML::Exception e) {
		Log::Error("SettingsSerializer::Deserialize - Error reading Settings file: " + e.msg);
		return false;
	}

	const auto& settingsNode = data["Settings"];

	if (!settingsNode) {
		Log::Error("SettingsSerializer::Deserialize - Error loading Settings file: " + filepath.string());

		return false;
	}

	Log::Trace("SettingsSerializer::Deserialize - Loading Settings from file: " + filepath.string());

	DeserializeApplicationSettings(settingsNode);
	DeserializeEditorSettings(settingsNode);
	DeserializeRenderingSettings(settingsNode);

	Log::Trace("SettingsSerializer::Deserialize - Loading complete");

	return true;
}

#pragma region - Serialization -

void SettingsSerializer::SerializeApplicationSettings(YAML::Emitter& out) {
	Log::Trace("SettingsSerializer::SerializeApplicationSettings - Serializing Application Settings");

	out << YAML::Key << "Application" << YAML::Value << YAML::BeginMap; // Application
	{
		const auto& application = m_Settings.Application;
		out << YAML::Key << "Name" << YAML::Value << application.Name;
		out << YAML::Key << "Version" << YAML::Value << YAML::BeginMap; // Version
		{
			const auto& version = application.Version;
			out << YAML::Key << "Major" << YAML::Value << version.Major;
			out << YAML::Key << "Minor" << YAML::Value << version.Minor;
			out << YAML::Key << "Patch" << YAML::Value << version.Patch;
		}
		out << YAML::EndMap; // Version

		out << YAML::Key << "Maximized" << YAML::Value << application.Maximized;
		out << YAML::Key << "DebugMode" << YAML::Value << application.DebugMode;
		out << YAML::Key << "EscapeClosesApp" << YAML::Value << application.EscapeClosesApp;
	}
	out << YAML::EndMap; // Application

	Log::Trace("SettingsSerializer::SerializeApplicationSettings - Application Settings Serialized");
}

void SettingsSerializer::SerializeEditorSettings(YAML::Emitter& out) {
	Log::Trace("SettingsSerializer::SerializeEditorSettings - Serializing Editor Settings");

	out << YAML::Key << "Editor" << YAML::Value << YAML::BeginMap; // Editor
	{
		const auto& editor = m_Settings.Editor;
		out << YAML::Key << "Appearance" << YAML::Value << YAML::BeginMap; // Appearance
		{
			const auto& appearance = editor.Appearance;
			out << YAML::Key << "Theme" << YAML::Value << Utilities::EditorThemeToString(appearance.Theme);
			out << YAML::Key << "FontSize" << YAML::Value << appearance.FontSize;
			out << YAML::Key << "UIScale" << YAML::Value << appearance.UIScale;
			out << YAML::Key << "ColumnWidth" << YAML::Value << appearance.ColumnWidth;
		}
		out << YAML::EndMap; // Appearance

		out << YAML::Key << "AutoSaveInterval" << YAML::Value << editor.AutoSaveInterval;
	}
	out << YAML::EndMap; // Editor

	Log::Trace("SettingsSerializer::SerializeEditorSettings - Editor Settings Serialized");
}

void SettingsSerializer::SerializeRenderingSettings(YAML::Emitter& out) {
	Log::Trace("SettingsSerializer::SerializeRenderingSettings - Serializing Rendering Settings");

	out << YAML::Key << "Rendering" << YAML::Value << YAML::BeginMap; // Rendering
	{
		const auto& rendering = m_Settings.Rendering;
		out << YAML::Key << "Engine" << YAML::Value << Utilities::RenderingEngineToString(rendering.Engine);
		out << YAML::Key << "Resolution" << YAML::Value << YAML::BeginMap; // Resolution
		{
			const auto& resolution = rendering.Resolution;
			out << YAML::Key << "Width" << YAML::Value << resolution.Width;
			out << YAML::Key << "Height" << YAML::Value << resolution.Height;
			out << YAML::Key << "Scale" << YAML::Value << resolution.Scale;
		}
		out << YAML::EndMap; // Resolution

		out << YAML::Key << "Fullscreen" << YAML::Value << rendering.Fullscreen;
		out << YAML::Key << "VSync" << YAML::Value << rendering.VSync;
	}
	out << YAML::EndMap; // Rendering

	Log::Trace("SettingsSerializer::SerializeRenderingSettings - Rendering Settings Serialized");
}

#pragma endregion

#pragma region - Deserialization -

void SettingsSerializer::DeserializeApplicationSettings(const YAML::Node& settingsNode) {
	Log::Trace("SettingsSerializer::DeserializeApplicationSettings - Deserializing Application Settings");

	if (const auto& applicationNode = settingsNode["Application"]) {
		auto& application = m_Settings.Application;

		if (const auto& nameNode = applicationNode["Name"]) {
			application.Name = nameNode.as<std::string>();
		}

		if (const auto& versionNode = applicationNode["Version"]) {
			auto& version = application.Version;

			if (const auto& majorNode = versionNode["Major"]) {
				version.Major = majorNode.as<int>();
			}

			if (const auto& minorNode = versionNode["Minor"]) {
				version.Minor = minorNode.as<int>();
			}

			if (const auto& patchNode = versionNode["Patch"]) {
				version.Patch = patchNode.as<int>();
			}
		}

		if (const auto& maximizedNode = applicationNode["Maximized"]) {
			application.Maximized = maximizedNode.as<bool>();
		}

		if (const auto& debugModeNode = applicationNode["DebugMode"]) {
			application.DebugMode = debugModeNode.as<bool>();
		}

		if (const auto& escapeClosesAppNode = applicationNode["EscapeClosesApp"]) {
			application.EscapeClosesApp = escapeClosesAppNode.as<bool>();
		}
	}

	Log::Trace("SettingsSerializer::DeserializeApplicationSettings - Application Settings Deserialized");
}

void SettingsSerializer::DeserializeEditorSettings(const YAML::Node& settingsNode) {
	Log::Trace("SettingsSerializer::DeserializeEditorSettings - Deserializing Editor Settings");

	if (const auto& editorNode = settingsNode["Editor"]) {
		auto& editor = m_Settings.Editor;

		if (const auto& appearanceNode = editorNode["Appearance"]) {
			auto& appearance = editor.Appearance;

			if (const auto& themeNode = appearanceNode["Theme"]) {
				appearance.Theme = Utilities::StringToEditorTheme(themeNode.as<std::string>());
			}

			if (const auto& fontSizeNode = appearanceNode["FontSize"]) {
				appearance.FontSize = fontSizeNode.as<int>();
			}

			if (const auto& uiScaleNode = appearanceNode["UIScale"]) {
				appearance.UIScale = uiScaleNode.as<float>();
			}

			if (const auto& columnWidthNode = appearanceNode["ColumnWidth"]) {
				appearance.ColumnWidth = columnWidthNode.as<float>();
			}
		}

		if (const auto& autoSaveIntervalNode = editorNode["AutoSaveInterval"]) {
			editor.AutoSaveInterval = autoSaveIntervalNode.as<int>();
		}
	}

	Log::Trace("SettingsSerializer::DeserializeEditorSettings - Editor Settings Deserialized");
}

void SettingsSerializer::DeserializeRenderingSettings(const YAML::Node& settingsNode) {
	Log::Trace("SettingsSerializer::DeserializeRenderingSettings - Deserializing Rendering Settings");

	if (const auto& renderingNode = settingsNode["Rendering"]) {
		auto& rendering = m_Settings.Rendering;

		if (const auto& engineNode = renderingNode["Engine"]) {
			rendering.Engine = Utilities::StringToRenderingEngine(engineNode.as<std::string>());
		}

		if (const auto& resolutionNode = renderingNode["Resolution"]) {
			auto& resolution = rendering.Resolution;

			if (const auto& widthNode = resolutionNode["Width"]) {
				resolution.Width = widthNode.as<int>();
			}

			if (const auto& heightNode = resolutionNode["Height"]) {
				resolution.Height = heightNode.as<int>();
			}

			if (const auto& scaleNode = resolutionNode["Scale"]) {
				resolution.Scale = scaleNode.as<float>();
			}
		}

		if (const auto& fullscreenNode = renderingNode["Fullscreen"]) {
			rendering.Fullscreen = fullscreenNode.as<bool>();
		}

		if (const auto& vSyncNode = renderingNode["VSync"]) {
			rendering.VSync = vSyncNode.as<bool>();
		}
	}

	Log::Trace("SettingsSerializer::DeserializeRenderingSettings - Rendering Settings Deserialized");
}

#pragma endregion
