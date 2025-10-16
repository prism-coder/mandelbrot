#include "ThemeSerializer.h"

#include "Core/Log.h"

#include "Utilities/Utilities.h"

#include <fstream>

namespace YAML {
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

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.r);
			node.push_back(rhs.g);
			node.push_back(rhs.b);
			node.push_back(rhs.a);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) return false;

			rhs.r = node[0].as<float>();
			rhs.g = node[1].as<float>();
			rhs.b = node[2].as<float>();
			rhs.a = node[3].as<float>();
			return true;
		}
	};

	static Emitter& operator<<(Emitter& out, const glm::vec2& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << EndSeq;
		return out;
	}

	static Emitter& operator<<(Emitter& out, const glm::vec4& v) {
		out << Flow;
		out << BeginSeq << v.r << v.g << v.b << v.a << EndSeq;
		return out;
	}
}

ThemeSerializer::ThemeSerializer(Theme& theme)
	: m_Theme(theme)
{
	Log::Trace("ThemeSerializer::ThemeSerializer - Setting up Theme Serializer");
}

bool ThemeSerializer::Serialize(const std::filesystem::path& filepath) {
	Log::Trace("ThemeSerializer::Serialize - Saving Theme");

	if (filepath.empty()) {
		Log::Error("ThemeSerializer::Serialize - Filepath is empty");
		return false;
	}

	Log::Trace("ThemeSerializer::Serialize - Serializing Theme into file: " + filepath.string());

	YAML::Emitter out;
	{
		out << YAML::BeginMap; // Root
		{
			out << YAML::Key << "Theme" << YAML::Value << YAML::BeginMap; // Theme
			{

				SerializeStyle(out);
				SerializeColors(out);
			}
			out << YAML::EndMap; // Theme
		}
		out << YAML::EndMap; // Root
	}

	std::ofstream fout(filepath);

	if (fout.bad()) {
		Log::Error("ThemeSerializer::Serialize - Error during Theme Serialization: Bad file");
		return false;
	}

	if (!out.good()) {
		Log::Error("ThemeSerializer::Serialize - Error during Theme Serialization: Bad format");
		return false;
	}

	fout << out.c_str();

	Log::Trace("ThemeSerializer::Serialize - Theme Serialization complete");

	return true;
}

bool ThemeSerializer::Deserialize(const std::filesystem::path& filepath) {
	YAML::Node data;

	try {
		Log::Trace("ThemeSerializer::Deserialize - Attempting to read Theme file: " + filepath.string());

		data = YAML::LoadFile(filepath.string());
	} catch (YAML::Exception e) {
		Log::Error("ThemeSerializer::Deserialize - Error reading Theme file: " + e.msg);

		return false;
	}

	if (const auto& themeNode = data["Theme"]) {
		Log::Trace("ThemeSerializer::Deserialize - Loading Theme from file: " + filepath.string());

		DeserializeStyle(themeNode);
		DeserializeColors(themeNode);

		Log::Trace("ThemeSerializer::Deserialize - Loading complete");

		return true;
	}

	Log::Error("ThemeSerializer::Deserialize - Error loading Theme file: " + filepath.string());

	return false;
}

void ThemeSerializer::SerializeStyle(YAML::Emitter& out) {
	Log::Trace("ThemeSerializer::SerializeStyle - Serializing Style");

	out << YAML::Key << "Style" << YAML::Value << YAML::BeginMap; // Style
	{
		const auto& style = m_Theme.Style;
		out << YAML::Key << "Alpha" << YAML::Value << style.Alpha;
		out << YAML::Key << "DisabledAlpha" << YAML::Value << style.DisabledAlpha;
		out << YAML::Key << "WindowPadding" << YAML::Value << style.WindowPadding;
		out << YAML::Key << "WindowRounding" << YAML::Value << style.WindowRounding;
		out << YAML::Key << "WindowBorderSize" << YAML::Value << style.WindowBorderSize;
		out << YAML::Key << "WindowMinSize" << YAML::Value << style.WindowMinSize;
		out << YAML::Key << "WindowTitleAlign" << YAML::Value << style.WindowTitleAlign;
		out << YAML::Key << "WindowMenuButtonPosition" << YAML::Value << Utilities::DirectionToString(style.WindowMenuButtonPosition);
		out << YAML::Key << "ChildRounding" << YAML::Value << style.ChildRounding;
		out << YAML::Key << "ChildBorderSize" << YAML::Value << style.ChildBorderSize;
		out << YAML::Key << "PopupRounding" << YAML::Value << style.PopupRounding;
		out << YAML::Key << "PopupBorderSize" << YAML::Value << style.PopupBorderSize;
		out << YAML::Key << "FramePadding" << YAML::Value << style.FramePadding;
		out << YAML::Key << "FrameRounding" << YAML::Value << style.FrameRounding;
		out << YAML::Key << "FrameBorderSize" << YAML::Value << style.FrameBorderSize;
		out << YAML::Key << "ItemSpacing" << YAML::Value << style.ItemSpacing;
		out << YAML::Key << "ItemInnerSpacing" << YAML::Value << style.ItemInnerSpacing;
		out << YAML::Key << "CellPadding" << YAML::Value << style.CellPadding;
		out << YAML::Key << "IndentSpacing" << YAML::Value << style.IndentSpacing;
		out << YAML::Key << "ColumnsMinSpacing" << YAML::Value << style.ColumnsMinSpacing;
		out << YAML::Key << "ScrollbarSize" << YAML::Value << style.ScrollbarSize;
		out << YAML::Key << "ScrollbarRounding" << YAML::Value << style.ScrollbarRounding;
		out << YAML::Key << "GrabMinSize" << YAML::Value << style.GrabMinSize;
		out << YAML::Key << "GrabRounding" << YAML::Value << style.GrabRounding;
		out << YAML::Key << "TabRounding" << YAML::Value << style.TabRounding;
		out << YAML::Key << "TabBorderSize" << YAML::Value << style.TabBorderSize;
		out << YAML::Key << "ColorButtonPosition" << YAML::Value << Utilities::DirectionToString(style.ColorButtonPosition);
		out << YAML::Key << "ButtonTextAlign" << YAML::Value << style.ButtonTextAlign;
		out << YAML::Key << "SelectableTextAlign" << YAML::Value << style.SelectableTextAlign;
	}
	out << YAML::EndMap; // Style

	Log::Trace("ThemeSerializer::SerializeStyle - Style Serialized");
}

void ThemeSerializer::SerializeColors(YAML::Emitter& out) {
	Log::Trace("ThemeSerializer::SerializeColors - Serializing Colors");

	out << YAML::Key << "Colors" << YAML::Value << YAML::BeginMap; // Colors
	{
		const auto& colors = m_Theme.Colors;
		out << YAML::Key << "Text" << YAML::Value << colors.Text;
		out << YAML::Key << "TextDisabled" << YAML::Value << colors.TextDisabled;
		out << YAML::Key << "WindowBg" << YAML::Value << colors.WindowBg;
		out << YAML::Key << "ChildBg" << YAML::Value << colors.ChildBg;
		out << YAML::Key << "PopupBg" << YAML::Value << colors.PopupBg;
		out << YAML::Key << "Border" << YAML::Value << colors.Border;
		out << YAML::Key << "BorderShadow" << YAML::Value << colors.BorderShadow;
		out << YAML::Key << "FrameBg" << YAML::Value << colors.FrameBg;
		out << YAML::Key << "FrameBgHovered" << YAML::Value << colors.FrameBgHovered;
		out << YAML::Key << "FrameBgActive" << YAML::Value << colors.FrameBgActive;
		out << YAML::Key << "TitleBg" << YAML::Value << colors.TitleBg;
		out << YAML::Key << "TitleBgActive" << YAML::Value << colors.TitleBgActive;
		out << YAML::Key << "TitleBgCollapsed" << YAML::Value << colors.TitleBgCollapsed;
		out << YAML::Key << "MenuBarBg" << YAML::Value << colors.MenuBarBg;
		out << YAML::Key << "ScrollbarBg" << YAML::Value << colors.ScrollbarBg;
		out << YAML::Key << "ScrollbarGrab" << YAML::Value << colors.ScrollbarGrab;
		out << YAML::Key << "ScrollbarGrabHovered" << YAML::Value << colors.ScrollbarGrabHovered;
		out << YAML::Key << "ScrollbarGrabActive" << YAML::Value << colors.ScrollbarGrabActive;
		out << YAML::Key << "CheckMark" << YAML::Value << colors.CheckMark;
		out << YAML::Key << "SliderGrab" << YAML::Value << colors.SliderGrab;
		out << YAML::Key << "SliderGrabActive" << YAML::Value << colors.SliderGrabActive;
		out << YAML::Key << "Button" << YAML::Value << colors.Button;
		out << YAML::Key << "ButtonHovered" << YAML::Value << colors.ButtonHovered;
		out << YAML::Key << "ButtonActive" << YAML::Value << colors.ButtonActive;
		out << YAML::Key << "Header" << YAML::Value << colors.Header;
		out << YAML::Key << "HeaderHovered" << YAML::Value << colors.HeaderHovered;
		out << YAML::Key << "HeaderActive" << YAML::Value << colors.HeaderActive;
		out << YAML::Key << "Separator" << YAML::Value << colors.Separator;
		out << YAML::Key << "SeparatorHovered" << YAML::Value << colors.SeparatorHovered;
		out << YAML::Key << "SeparatorActive" << YAML::Value << colors.SeparatorActive;
		out << YAML::Key << "ResizeGrip" << YAML::Value << colors.ResizeGrip;
		out << YAML::Key << "ResizeGripHovered" << YAML::Value << colors.ResizeGripHovered;
		out << YAML::Key << "ResizeGripActive" << YAML::Value << colors.ResizeGripActive;
		out << YAML::Key << "Tab" << YAML::Value << colors.Tab;
		out << YAML::Key << "TabHovered" << YAML::Value << colors.TabHovered;
		out << YAML::Key << "TabActive" << YAML::Value << colors.TabActive;
		out << YAML::Key << "TabUnfocused" << YAML::Value << colors.TabUnfocused;
		out << YAML::Key << "TabUnfocusedActive" << YAML::Value << colors.TabUnfocusedActive;
		out << YAML::Key << "PlotLines" << YAML::Value << colors.PlotLines;
		out << YAML::Key << "PlotLinesHovered" << YAML::Value << colors.PlotLinesHovered;
		out << YAML::Key << "PlotHistogram" << YAML::Value << colors.PlotHistogram;
		out << YAML::Key << "PlotHistogramHovered" << YAML::Value << colors.PlotHistogramHovered;
		out << YAML::Key << "TableHeaderBg" << YAML::Value << colors.TableHeaderBg;
		out << YAML::Key << "TableBorderStrong" << YAML::Value << colors.TableBorderStrong;
		out << YAML::Key << "TableBorderLight" << YAML::Value << colors.TableBorderLight;
		out << YAML::Key << "TableRowBg" << YAML::Value << colors.TableRowBg;
		out << YAML::Key << "TableRowBgAlt" << YAML::Value << colors.TableRowBgAlt;
		out << YAML::Key << "TextSelectedBg" << YAML::Value << colors.TextSelectedBg;
		out << YAML::Key << "DragDropTarget" << YAML::Value << colors.DragDropTarget;
		out << YAML::Key << "NavHighlight" << YAML::Value << colors.NavHighlight;
		out << YAML::Key << "NavWindowingHighlight" << YAML::Value << colors.NavWindowingHighlight;
		out << YAML::Key << "NavWindowingDimBg" << YAML::Value << colors.NavWindowingDimBg;
		out << YAML::Key << "ModalWindowDimBg" << YAML::Value << colors.ModalWindowDimBg;
	}
	out << YAML::EndMap; // Colors

	Log::Trace("ThemeSerializer::SerializeColors - Colors Serialized");
}

void ThemeSerializer::DeserializeStyle(const YAML::Node& themeNode) {
	Log::Trace("ThemeSerializer::DeserializeStyle - Deserializing Style");

	if (const auto& styleNode = themeNode["Style"]) {
		auto& style = m_Theme.Style;
		
		if (const auto AlphaNode = styleNode["Alpha"]) {
			style.Alpha = AlphaNode.as<float>();
		}

		if (const auto DisabledAlphaNode = styleNode["DisabledAlpha"]) {
			style.DisabledAlpha = DisabledAlphaNode.as<float>();
		}

		if (const auto WindowPaddingNode = styleNode["WindowPadding"]) {
			style.WindowPadding = WindowPaddingNode.as<glm::vec2>();
		}

		if (const auto WindowRoundingNode = styleNode["WindowRounding"]) {
			style.WindowRounding = WindowRoundingNode.as<float>();
		}

		if (const auto WindowBorderSizeNode = styleNode["WindowBorderSize"]) {
			style.WindowBorderSize = WindowBorderSizeNode.as<float>();
		}

		if (const auto WindowMinSizeNode = styleNode["WindowMinSize"]) {
			style.WindowMinSize = WindowMinSizeNode.as<glm::vec2>();
		}

		if (const auto WindowTitleAlignNode = styleNode["WindowTitleAlign"]) {
			style.WindowTitleAlign = WindowTitleAlignNode.as<glm::vec2>();
		}

		if (const auto WindowMenuButtonPositionNode = styleNode["WindowMenuButtonPosition"]) {
			style.WindowMenuButtonPosition = Utilities::StringToDirection(WindowMenuButtonPositionNode.as<std::string>());
		}

		if (const auto ChildRoundingNode = styleNode["ChildRounding"]) {
			style.ChildRounding = ChildRoundingNode.as<float>();
		}

		if (const auto ChildBorderSizeNode = styleNode["ChildBorderSize"]) {
			style.ChildBorderSize = ChildBorderSizeNode.as<float>();
		}

		if (const auto PopupRoundingNode = styleNode["PopupRounding"]) {
			style.PopupRounding = PopupRoundingNode.as<float>();
		}

		if (const auto PopupBorderSizeNode = styleNode["PopupBorderSize"]) {
			style.PopupBorderSize = PopupBorderSizeNode.as<float>();
		}

		if (const auto FramePaddingNode = styleNode["FramePadding"]) {
			style.FramePadding = FramePaddingNode.as<glm::vec2>();
		}

		if (const auto FrameRoundingNode = styleNode["FrameRounding"]) {
			style.FrameRounding = FrameRoundingNode.as<float>();
		}

		if (const auto FrameBorderSizeNode = styleNode["FrameBorderSize"]) {
			style.FrameBorderSize = FrameBorderSizeNode.as<float>();
		}

		if (const auto ItemSpacingNode = styleNode["ItemSpacing"]) {
			style.ItemSpacing = ItemSpacingNode.as<glm::vec2>();
		}

		if (const auto ItemInnerSpacingNode = styleNode["ItemInnerSpacing"]) {
			style.ItemInnerSpacing = ItemInnerSpacingNode.as<glm::vec2>();
		}

		if (const auto CellPaddingNode = styleNode["CellPadding"]) {
			style.CellPadding = CellPaddingNode.as<glm::vec2>();
		}

		if (const auto IndentSpacingNode = styleNode["IndentSpacing"]) {
			style.IndentSpacing = IndentSpacingNode.as<float>();
		}

		if (const auto ColumnsMinSpacingNode = styleNode["ColumnsMinSpacing"]) {
			style.ColumnsMinSpacing = ColumnsMinSpacingNode.as<float>();
		}

		if (const auto ScrollbarSizeNode = styleNode["ScrollbarSize"]) {
			style.ScrollbarSize = ScrollbarSizeNode.as<float>();
		}

		if (const auto ScrollbarRoundingNode = styleNode["ScrollbarRounding"]) {
			style.ScrollbarRounding = ScrollbarRoundingNode.as<float>();
		}

		if (const auto GrabMinSizeNode = styleNode["GrabMinSize"]) {
			style.GrabMinSize = GrabMinSizeNode.as<float>();
		}

		if (const auto GrabRoundingNode = styleNode["GrabRounding"]) {
			style.GrabRounding = GrabRoundingNode.as<float>();
		}

		if (const auto TabRoundingNode = styleNode["TabRounding"]) {
			style.TabRounding = TabRoundingNode.as<float>();
		}

		if (const auto TabBorderSizeNode = styleNode["TabBorderSize"]) {
			style.TabBorderSize = TabBorderSizeNode.as<float>();
		}

		if (const auto ColorButtonPositionNode = styleNode["ColorButtonPosition"]) {
			style.ColorButtonPosition = Utilities::StringToDirection(ColorButtonPositionNode.as<std::string>());
		}

		if (const auto ButtonTextAlignNode = styleNode["ButtonTextAlign"]) {
			style.ButtonTextAlign = ButtonTextAlignNode.as<glm::vec2>();
		}

		if (const auto SelectableTextAlignNode = styleNode["SelectableTextAlign"]) {
			style.SelectableTextAlign = SelectableTextAlignNode.as<glm::vec2>();
		}
	}

	Log::Trace("ThemeSerializer::DeserializeStyle - Style Deserialized");
}

void ThemeSerializer::DeserializeColors(const YAML::Node& themeNode) {
	Log::Trace("ThemeSerializer::DeserializeColors - Deserializing Colors");

	if (const auto& colorsNode = themeNode["Colors"]) {
		auto& colors = m_Theme.Colors;
		
		if (const auto& TextNode = colorsNode["Text"]) {
			colors.Text = TextNode.as<glm::vec4>();
		}

		if (const auto& TextDisabledNode = colorsNode["TextDisabled"]) {
			colors.TextDisabled = TextDisabledNode.as<glm::vec4>();
		}

		if (const auto& WindowBgNode = colorsNode["WindowBg"]) {
			colors.WindowBg = WindowBgNode.as<glm::vec4>();
		}

		if (const auto& ChildBgNode = colorsNode["ChildBg"]) {
			colors.ChildBg = ChildBgNode.as<glm::vec4>();
		}

		if (const auto& PopupBgNode = colorsNode["PopupBg"]) {
			colors.PopupBg = PopupBgNode.as<glm::vec4>();
		}

		if (const auto& BorderNode = colorsNode["Border"]) {
			colors.Border = BorderNode.as<glm::vec4>();
		}

		if (const auto& BorderShadowNode = colorsNode["BorderShadow"]) {
			colors.BorderShadow = BorderShadowNode.as<glm::vec4>();
		}

		if (const auto& FrameBgNode = colorsNode["FrameBg"]) {
			colors.FrameBg = FrameBgNode.as<glm::vec4>();
		}

		if (const auto& FrameBgHoveredNode = colorsNode["FrameBgHovered"]) {
			colors.FrameBgHovered = FrameBgHoveredNode.as<glm::vec4>();
		}

		if (const auto& FrameBgActiveNode = colorsNode["FrameBgActive"]) {
			colors.FrameBgActive = FrameBgActiveNode.as<glm::vec4>();
		}

		if (const auto& TitleBgNode = colorsNode["TitleBg"]) {
			colors.TitleBg = TitleBgNode.as<glm::vec4>();
		}

		if (const auto& TitleBgActiveNode = colorsNode["TitleBgActive"]) {
			colors.TitleBgActive = TitleBgActiveNode.as<glm::vec4>();
		}

		if (const auto& TitleBgCollapsedNode = colorsNode["TitleBgCollapsed"]) {
			colors.TitleBgCollapsed = TitleBgCollapsedNode.as<glm::vec4>();
		}

		if (const auto& MenuBarBgNode = colorsNode["MenuBarBg"]) {
			colors.MenuBarBg = MenuBarBgNode.as<glm::vec4>();
		}

		if (const auto& ScrollbarBgNode = colorsNode["ScrollbarBg"]) {
			colors.ScrollbarBg = ScrollbarBgNode.as<glm::vec4>();
		}

		if (const auto& ScrollbarGrabNode = colorsNode["ScrollbarGrab"]) {
			colors.ScrollbarGrab = ScrollbarGrabNode.as<glm::vec4>();
		}

		if (const auto& ScrollbarGrabHoveredNode = colorsNode["ScrollbarGrabHovered"]) {
			colors.ScrollbarGrabHovered = ScrollbarGrabHoveredNode.as<glm::vec4>();
		}

		if (const auto& ScrollbarGrabActiveNode = colorsNode["ScrollbarGrabActive"]) {
			colors.ScrollbarGrabActive = ScrollbarGrabActiveNode.as<glm::vec4>();
		}

		if (const auto& CheckMarkNode = colorsNode["CheckMark"]) {
			colors.CheckMark = CheckMarkNode.as<glm::vec4>();
		}

		if (const auto& SliderGrabNode = colorsNode["SliderGrab"]) {
			colors.SliderGrab = SliderGrabNode.as<glm::vec4>();
		}

		if (const auto& SliderGrabActiveNode = colorsNode["SliderGrabActive"]) {
			colors.SliderGrabActive = SliderGrabActiveNode.as<glm::vec4>();
		}

		if (const auto& ButtonNode = colorsNode["Button"]) {
			colors.Button = ButtonNode.as<glm::vec4>();
		}

		if (const auto& ButtonHoveredNode = colorsNode["ButtonHovered"]) {
			colors.ButtonHovered = ButtonHoveredNode.as<glm::vec4>();
		}

		if (const auto& ButtonActiveNode = colorsNode["ButtonActive"]) {
			colors.ButtonActive = ButtonActiveNode.as<glm::vec4>();
		}

		if (const auto& HeaderNode = colorsNode["Header"]) {
			colors.Header = HeaderNode.as<glm::vec4>();
		}

		if (const auto& HeaderHoveredNode = colorsNode["HeaderHovered"]) {
			colors.HeaderHovered = HeaderHoveredNode.as<glm::vec4>();
		}

		if (const auto& HeaderActiveNode = colorsNode["HeaderActive"]) {
			colors.HeaderActive = HeaderActiveNode.as<glm::vec4>();
		}

		if (const auto& SeparatorNode = colorsNode["Separator"]) {
			colors.Separator = SeparatorNode.as<glm::vec4>();
		}

		if (const auto& SeparatorHoveredNode = colorsNode["SeparatorHovered"]) {
			colors.SeparatorHovered = SeparatorHoveredNode.as<glm::vec4>();
		}

		if (const auto& SeparatorActiveNode = colorsNode["SeparatorActive"]) {
			colors.SeparatorActive = SeparatorActiveNode.as<glm::vec4>();
		}

		if (const auto& ResizeGripNode = colorsNode["ResizeGrip"]) {
			colors.ResizeGrip = ResizeGripNode.as<glm::vec4>();
		}

		if (const auto& ResizeGripHoveredNode = colorsNode["ResizeGripHovered"]) {
			colors.ResizeGripHovered = ResizeGripHoveredNode.as<glm::vec4>();
		}

		if (const auto& ResizeGripActiveNode = colorsNode["ResizeGripActive"]) {
			colors.ResizeGripActive = ResizeGripActiveNode.as<glm::vec4>();
		}

		if (const auto& TabNode = colorsNode["Tab"]) {
			colors.Tab = TabNode.as<glm::vec4>();
		}

		if (const auto& TabHoveredNode = colorsNode["TabHovered"]) {
			colors.TabHovered = TabHoveredNode.as<glm::vec4>();
		}

		if (const auto& TabActiveNode = colorsNode["TabActive"]) {
			colors.TabActive = TabActiveNode.as<glm::vec4>();
		}

		if (const auto& TabUnfocusedNode = colorsNode["TabUnfocused"]) {
			colors.TabUnfocused = TabUnfocusedNode.as<glm::vec4>();
		}

		if (const auto& TabUnfocusedActiveNode = colorsNode["TabUnfocusedActive"]) {
			colors.TabUnfocusedActive = TabUnfocusedActiveNode.as<glm::vec4>();
		}

		if (const auto& PlotLinesNode = colorsNode["PlotLines"]) {
			colors.PlotLines = PlotLinesNode.as<glm::vec4>();
		}

		if (const auto& PlotLinesHoveredNode = colorsNode["PlotLinesHovered"]) {
			colors.PlotLinesHovered = PlotLinesHoveredNode.as<glm::vec4>();
		}

		if (const auto& PlotHistogramNode = colorsNode["PlotHistogram"]) {
			colors.PlotHistogram = PlotHistogramNode.as<glm::vec4>();
		}

		if (const auto& PlotHistogramHoveredNode = colorsNode["PlotHistogramHovered"]) {
			colors.PlotHistogramHovered = PlotHistogramHoveredNode.as<glm::vec4>();
		}

		if (const auto& TableHeaderBgNode = colorsNode["TableHeaderBg"]) {
			colors.TableHeaderBg = TableHeaderBgNode.as<glm::vec4>();
		}

		if (const auto& TableBorderStrongNode = colorsNode["TableBorderStrong"]) {
			colors.TableBorderStrong = TableBorderStrongNode.as<glm::vec4>();
		}

		if (const auto& TableBorderLightNode = colorsNode["TableBorderLight"]) {
			colors.TableBorderLight = TableBorderLightNode.as<glm::vec4>();
		}

		if (const auto& TableRowBgNode = colorsNode["TableRowBg"]) {
			colors.TableRowBg = TableRowBgNode.as<glm::vec4>();
		}

		if (const auto& TableRowBgAltNode = colorsNode["TableRowBgAlt"]) {
			colors.TableRowBgAlt = TableRowBgAltNode.as<glm::vec4>();
		}

		if (const auto& TextSelectedBgNode = colorsNode["TextSelectedBg"]) {
			colors.TextSelectedBg = TextSelectedBgNode.as<glm::vec4>();
		}

		if (const auto& DragDropTargetNode = colorsNode["DragDropTarget"]) {
			colors.DragDropTarget = DragDropTargetNode.as<glm::vec4>();
		}

		if (const auto& NavHighlightNode = colorsNode["NavHighlight"]) {
			colors.NavHighlight = NavHighlightNode.as<glm::vec4>();
		}

		if (const auto& NavWindowingHighlightNode = colorsNode["NavWindowingHighlight"]) {
			colors.NavWindowingHighlight = NavWindowingHighlightNode.as<glm::vec4>();
		}

		if (const auto& NavWindowingDimBgNode = colorsNode["NavWindowingDimBg"]) {
			colors.NavWindowingDimBg = NavWindowingDimBgNode.as<glm::vec4>();
		}

		if (const auto& ModalWindowDimBgNode = colorsNode["ModalWindowDimBg"]) {
			colors.ModalWindowDimBg = ModalWindowDimBgNode.as<glm::vec4>();
		}
	}

	Log::Trace("ThemeSerializer::DeserializeColors - Colors Deserialized");
}
