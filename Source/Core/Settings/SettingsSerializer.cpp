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

	// Check if the provided file path is empty, which would indicate an invalid path for saving settings.
	if (filepath.empty()) {
		Log::Error("SettingsSerializer::Serialize - Filepath is empty");

		// Return false to indicate failure due to an invalid file path.
		return false;
	}

	Log::Trace("SettingsSerializer::Serialize - Serializing Settings into file: " + filepath.string());

	// Create a YAML emitter to construct the YAML representation of the settings.
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
				SerializeNavigationSettings(out);
				SerializeExportSettings(out);
			}
			out << YAML::EndMap; // Settings
		}
		out << YAML::EndMap; // Root
	}

	// Open a file stream to write the serialized settings to the specified file path.
	std::ofstream fout(filepath);

	// Check if the file stream is in a bad state, which would indicate an issue with opening or writing to the file.
	if (fout.bad()) {
		Log::Error("SettingsSerializer::Serialize - Error during Settings Serialization: Bad file");

		// Return false to indicate failure due to an issue with the file stream.
		return false;
	}

	// Check if the YAML emitter is not in a good state, which would indicate that the serialization process encountered an error.
	if (!out.good()) {
		Log::Error("SettingsSerializer::Serialize - Error during Settings Serialization: Bad format");

		// Return false to indicate failure due to an issue with the YAML emitter.
		return false;
	}

	// Write the serialized YAML string to the file.
	fout << out.c_str();

	Log::Trace("SettingsSerializer::Serialize - Settings Serialization complete");

	// Return true to indicate that the settings were successfully serialized and saved to the file.
	return true;
}

bool SettingsSerializer::Deserialize(const std::filesystem::path& filepath) {
	// Data structure to hold the loaded YAML data from the file.
	YAML::Node data;

	try {
		Log::Trace("SettingsSerializer::Deserialize - Attempting to read Settings file: " + filepath.string());

		// Load the YAML data from the specified file path. If the file cannot be read or parsed, an exception will be thrown.
		data = YAML::LoadFile(filepath.string());
	} catch (YAML::Exception e) {
		Log::Error("SettingsSerializer::Deserialize - Error reading Settings file: " + e.msg);

		// Return false to indicate failure due to an error while reading the file.
		return false;
	}

	// Retrieve the "Settings" node from the loaded YAML data.
	const auto& settingsNode = data["Settings"];

	// Check if the "Settings" node is not present in the YAML data, which would indicate an issue with the file format or content.
	if (!settingsNode) {
		Log::Error("SettingsSerializer::Deserialize - Error loading Settings file: " + filepath.string());

		// Return false to indicate failure due to missing "Settings" node in the YAML data.
		return false;
	}

	Log::Trace("SettingsSerializer::Deserialize - Loading Settings from file: " + filepath.string());

	// Deserialize the settings from the corresponding nodes in the YAML data and update the current settings accordingly.
	DeserializeApplicationSettings(settingsNode);
	DeserializeEditorSettings(settingsNode);
	DeserializeRenderingSettings(settingsNode);
	DeserializeNavigationSettings(settingsNode);
	DeserializeExportSettings(settingsNode);

	Log::Trace("SettingsSerializer::Deserialize - Loading complete");

	// Return true to indicate that the settings were successfully deserialized and loaded from the file.
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
		out << YAML::Key << "LogToFile" << YAML::Value << application.LogToFile;
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

		out << YAML::Key << "Windows" << YAML::Value << YAML::BeginMap; // Windows
		{
			const auto& windows = editor.Windows;
			out << YAML::Key << "ShowAbout"      << YAML::Value << windows.ShowAbout;
			out << YAML::Key << "ShowInspector"  << YAML::Value << windows.ShowInspector;
			out << YAML::Key << "ShowProject"    << YAML::Value << windows.ShowProject;
			out << YAML::Key << "ShowSettings"   << YAML::Value << windows.ShowSettings;
			out << YAML::Key << "ShowStatistics" << YAML::Value << windows.ShowStatistics;
			out << YAML::Key << "ShowViewport"   << YAML::Value << windows.ShowViewport;
		}
		out << YAML::EndMap; // Windows

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

		out << YAML::Key << "Mode" << YAML::Value << Utilities::WindowModeToString(rendering.Mode);
		out << YAML::Key << "VSync" << YAML::Value << rendering.VSync;
		out << YAML::Key << "LockFramerate" << YAML::Value << rendering.LockFramerate;
		out << YAML::Key << "TargetFrameRate" << YAML::Value << rendering.TargetFrameRate;
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

		if (const auto& logToFileNode = applicationNode["LogToFile"]) {
			application.LogToFile = logToFileNode.as<bool>();
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

		if (const auto& windowsNode = editorNode["Windows"]) {
			auto& windows = editor.Windows;

			if (const auto& showAboutNode = windowsNode["ShowAbout"]) {
				windows.ShowAbout = showAboutNode.as<bool>();
			}

			if (const auto& showInspectorNode = windowsNode["ShowInspector"]) {
				windows.ShowInspector = showInspectorNode.as<bool>();
			}

			if (const auto& showProjectNode = windowsNode["ShowProject"]) {
				windows.ShowProject = showProjectNode.as<bool>();
			}

			if (const auto& showSettingsNode = windowsNode["ShowSettings"]) {
				windows.ShowSettings = showSettingsNode.as<bool>();
			}

			if (const auto& showStatisticsNode = windowsNode["ShowStatistics"]) {
				windows.ShowStatistics = showStatisticsNode.as<bool>();
			}

			if (const auto& showViewportNode = windowsNode["ShowViewport"]) {
				windows.ShowViewport = showViewportNode.as<bool>();
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

		// Legacy: map old 'Fullscreen' bool to the new WindowMode enum.
		if (const auto& fullscreenNode = renderingNode["Fullscreen"]) {
			if (fullscreenNode.as<bool>()) {
				rendering.Mode = WindowMode::Fullscreen;
			}
		}

		if (const auto& modeNode = renderingNode["Mode"]) {
			rendering.Mode = Utilities::StringToWindowMode(modeNode.as<std::string>());
		}

		if (const auto& vSyncNode = renderingNode["VSync"]) {
			rendering.VSync = vSyncNode.as<bool>();
		}

		if (const auto& lockFramerateNode = renderingNode["LockFramerate"]) {
			rendering.LockFramerate = lockFramerateNode.as<bool>();
		}

		if (const auto& targetFrameRateNode = renderingNode["TargetFrameRate"]) {
			rendering.TargetFrameRate = targetFrameRateNode.as<int>();
		}
	}

	Log::Trace("SettingsSerializer::DeserializeRenderingSettings - Rendering Settings Deserialized");
}

void SettingsSerializer::SerializeNavigationSettings(YAML::Emitter& out) {
	Log::Trace("SettingsSerializer::SerializeNavigationSettings - Serializing Navigation Settings");

	out << YAML::Key << "Navigation" << YAML::Value << YAML::BeginMap; // Navigation
	{
		const auto& nav = m_Settings.Navigation;
		out << YAML::Key << "MovementSpeed" << YAML::Value << nav.MovementSpeed;
		out << YAML::Key << "RotationSpeed" << YAML::Value << nav.RotationSpeed;
		out << YAML::Key << "ZoomSpeed" << YAML::Value << nav.ZoomSpeed;
		out << YAML::Key << "PowerSpeed" << YAML::Value << nav.PowerSpeed;
		out << YAML::Key << "Smoothing" << YAML::Value << nav.Smoothing;
		out << YAML::Key << "InvertZoom" << YAML::Value << nav.InvertZoom;
	}
	out << YAML::EndMap; // Navigation

	Log::Trace("SettingsSerializer::SerializeNavigationSettings - Navigation Settings Serialized");
}

void SettingsSerializer::SerializeExportSettings(YAML::Emitter& out) {
	Log::Trace("SettingsSerializer::SerializeExportSettings - Serializing Export Settings");

	out << YAML::Key << "Export" << YAML::Value << YAML::BeginMap; // Export
	{
		const auto& exp = m_Settings.Export;
		out << YAML::Key << "ImageFormat" << YAML::Value << Utilities::ExportImageFormatToString(exp.ImageFormat);
		out << YAML::Key << "ImageQuality" << YAML::Value << exp.ImageQuality;
		out << YAML::Key << "Folder" << YAML::Value << exp.Folder.string();
	}
	out << YAML::EndMap; // Export

	Log::Trace("SettingsSerializer::SerializeExportSettings - Export Settings Serialized");
}

void SettingsSerializer::DeserializeNavigationSettings(const YAML::Node& settingsNode) {
	Log::Trace("SettingsSerializer::DeserializeNavigationSettings - Deserializing Navigation Settings");

	if (const auto& navNode = settingsNode["Navigation"]) {
		auto& nav = m_Settings.Navigation;

		if (const auto& movementSpeedNode = navNode["MovementSpeed"]) {
			nav.MovementSpeed = movementSpeedNode.as<float>();
		}

		if (const auto& rotationSpeedNode = navNode["RotationSpeed"]) {
			nav.RotationSpeed = rotationSpeedNode.as<float>();
		}

		if (const auto& zoomSpeedNode = navNode["ZoomSpeed"]) {
			nav.ZoomSpeed = zoomSpeedNode.as<float>();
		}

		if (const auto& powerSpeedNode = navNode["PowerSpeed"]) {
			nav.PowerSpeed = powerSpeedNode.as<float>();
		}

		if (const auto& smoothingNode = navNode["Smoothing"]) {
			nav.Smoothing = smoothingNode.as<float>();
		}

		if (const auto& invertZoomNode = navNode["InvertZoom"]) {
			nav.InvertZoom = invertZoomNode.as<bool>();
		}
	}

	Log::Trace("SettingsSerializer::DeserializeNavigationSettings - Navigation Settings Deserialized");
}

void SettingsSerializer::DeserializeExportSettings(const YAML::Node& settingsNode) {
	Log::Trace("SettingsSerializer::DeserializeExportSettings - Deserializing Export Settings");

	if (const auto& exportNode = settingsNode["Export"]) {
		auto& exp = m_Settings.Export;

		if (const auto& imageFormatNode = exportNode["ImageFormat"]) {
			exp.ImageFormat = Utilities::StringToExportImageFormat(imageFormatNode.as<std::string>());
		}

		if (const auto& imageQualityNode = exportNode["ImageQuality"]) {
			exp.ImageQuality = imageQualityNode.as<int>();
		}

		if (const auto& folderNode = exportNode["Folder"]) {
			exp.Folder = folderNode.as<std::string>();
		}
	}

	Log::Trace("SettingsSerializer::DeserializeExportSettings - Export Settings Deserialized");
}

#pragma endregion
