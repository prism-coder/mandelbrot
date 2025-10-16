#pragma once

#include "glm/glm.hpp"

enum class Direction {
	None = -1,
	Left,
	Right,
	Up,
	Down
};

struct Style {
	float Alpha = 1.0f;
	float DisabledAlpha = 0.6f;
	glm::vec2 WindowPadding = { 10.0f, 10.0f };
	float WindowRounding = 0.0f;
	float WindowBorderSize = 1.0f;
	glm::vec2 WindowMinSize = { 32.0f, 32.0f };
	glm::vec2 WindowTitleAlign = { 0.5f, 0.5f };
	Direction WindowMenuButtonPosition = Direction::Left;
	float ChildRounding = 6.0f;
	float ChildBorderSize = 1.0f;
	float PopupRounding = 6.0f;
	float PopupBorderSize = 1.0f;
	glm::vec2 FramePadding = { 8.0f, 6.0f };
	float FrameRounding = 6.0f;
	float FrameBorderSize = 1.0f;
	glm::vec2 ItemSpacing = { 6.0f, 6.0f };
	glm::vec2 ItemInnerSpacing = { 4.0f, 4.0f };
	glm::vec2 CellPadding = { 4.0f, 2.0f };
	float IndentSpacing = 11.0f;
	float ColumnsMinSpacing = 6.0f;
	float ScrollbarSize = 14.0f;
	float ScrollbarRounding = 6.0f;
	float GrabMinSize = 10.0f;
	float GrabRounding = 6.0f;
	float TabRounding = 6.0f;
	float TabBorderSize = 1.0f;
	Direction ColorButtonPosition = Direction::Right;
	glm::vec2 ButtonTextAlign = { 0.5f, 0.5f };
	glm::vec2 SelectableTextAlign = { 0.0f, 0.0f };
};

struct Colors {
	glm::vec4 Text = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 TextDisabled = { 0.501960813999176f, 0.501960813999176f, 0.501960813999176f, 1.0f };
	glm::vec4 WindowBg = { 0.08235294371843338f, 0.08235294371843338f, 0.08235294371843338f, 1.0f};
	glm::vec4 ChildBg = { 0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f};
	glm::vec4 PopupBg = { 0.196078434586525f, 0.196078434586525f, 0.196078434586525f, 1.0f};
	glm::vec4 Border = { 0.1019607856869698f, 0.1019607856869698f, 0.1019607856869698f, 1.0f};
	glm::vec4 BorderShadow = { 0.0f, 0.0f, 0.0f, 0.0f};
	glm::vec4 FrameBg = { 0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 1.0f};
	glm::vec4 FrameBgHovered = { 0.09019608050584793f, 0.09019608050584793f, 0.09019608050584793f, 1.0f};
	glm::vec4 FrameBgActive = { 0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 1.0f};
	glm::vec4 TitleBg = { 0.08235294371843338f, 0.08235294371843338f, 0.08235294371843338f, 1.0f};
	glm::vec4 TitleBgActive = { 0.08235294371843338f, 0.08235294371843338f, 0.08235294371843338f, 1.0f};
	glm::vec4 TitleBgCollapsed = { 0.1529411822557449f, 0.1529411822557449f, 0.1529411822557449f, 1.0f};
	glm::vec4 MenuBarBg = { 0.0f, 0.0f, 0.0f, 0.0f};
	glm::vec4 ScrollbarBg = { 0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f};
	glm::vec4 ScrollbarGrab = { 0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f};
	glm::vec4 ScrollbarGrabHovered = { 0.4117647111415863f, 0.4117647111415863f, 0.4117647111415863f, 1.0f};
	glm::vec4 ScrollbarGrabActive = { 0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f};
	glm::vec4 CheckMark = { 0.7529411911964417f, 0.7529411911964417f, 0.7529411911964417f, 1.0f};
	glm::vec4 SliderGrab = { 0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 0.699999988079071f};
	glm::vec4 SliderGrabActive = { 0.658823549747467f, 0.658823549747467f, 0.658823549747467f, 1.0f};
	glm::vec4 Button = { 0.2196078449487686f, 0.2196078449487686f, 0.2196078449487686f, 0.7839999794960022f};
	glm::vec4 ButtonHovered = { 0.2745098173618317f, 0.2745098173618317f, 0.2745098173618317f, 1.0f};
	glm::vec4 ButtonActive = { 0.2196078449487686f, 0.2196078449487686f, 0.2196078449487686f, 0.5879999995231628f};
	glm::vec4 Header = { 0.1843137294054031f, 0.1843137294054031f, 0.1843137294054031f, 1.0f};
	glm::vec4 HeaderHovered = { 0.1843137294054031f, 0.1843137294054031f, 0.1843137294054031f, 1.0f};
	glm::vec4 HeaderActive = { 0.1843137294054031f, 0.1843137294054031f, 0.1843137294054031f, 1.0f};
	glm::vec4 Separator = { 0.1019607856869698f, 0.1019607856869698f, 0.1019607856869698f, 1.0f};
	glm::vec4 SeparatorHovered = { 0.1529411822557449f, 0.7254902124404907f, 0.9490196108818054f, 0.5879999995231628f};
	glm::vec4 SeparatorActive = { 0.1529411822557449f, 0.7254902124404907f, 0.9490196108818054f, 1.0f};
	glm::vec4 ResizeGrip = { 0.9098039269447327f, 0.9098039269447327f, 0.9098039269447327f, 0.25f};
	glm::vec4 ResizeGripHovered = { 0.8117647171020508f, 0.8117647171020508f, 0.8117647171020508f, 0.6700000166893005f};
	glm::vec4 ResizeGripActive = { 0.4588235318660736f, 0.4588235318660736f, 0.4588235318660736f, 0.949999988079071f};
	glm::vec4 Tab = { 0.08235294371843338f, 0.08235294371843338f, 0.08235294371843338f, 1.0f};
	glm::vec4 TabHovered = { 1.0f, 0.8823529481887817f, 0.529411792755127f, 0.1180000007152557f};
	glm::vec4 TabActive = { 1.0f, 0.8823529481887817f, 0.529411792755127f, 0.2349999994039536f};
	glm::vec4 TabUnfocused = { 0.08235294371843338f, 0.08235294371843338f, 0.08235294371843338f, 1.0f};
	glm::vec4 TabUnfocusedActive = { 1.0f, 0.8823529481887817f, 0.529411792755127f, 0.1180000007152557f};
	glm::vec4 PlotLines = { 0.6117647290229797f, 0.6117647290229797f, 0.6117647290229797f, 1.0f};
	glm::vec4 PlotLinesHovered = { 1.0f, 0.4313725531101227f, 0.3490196168422699f, 1.0f};
	glm::vec4 PlotHistogram = { 0.9019607901573181f, 0.7019608020782471f, 0.0f, 1.0f};
	glm::vec4 PlotHistogramHovered = { 1.0f, 0.6000000238418579f, 0.0f, 1.0f};
	glm::vec4 TableHeaderBg = { 0.1843137294054031f, 0.1843137294054031f, 0.1843137294054031f, 1.0f};
	glm::vec4 TableBorderStrong = { 0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f};
	glm::vec4 TableBorderLight = { 0.1019607856869698f, 0.1019607856869698f, 0.1019607856869698f, 1.0f};
	glm::vec4 TableRowBg = { 0.0f, 0.0f, 0.0f, 0.0f};
	glm::vec4 TableRowBgAlt = { 1.0f, 1.0f, 1.0f, 0.05999999865889549f};
	glm::vec4 TextSelectedBg = { 0.1529411822557449f, 0.7254902124404907f, 0.9490196108818054f, 0.3499999940395355f};
	glm::vec4 DragDropTarget = { 1.0f, 1.0f, 0.0f, 0.8999999761581421f};
	glm::vec4 NavHighlight = { 0.1529411822557449f, 0.7254902124404907f, 0.9490196108818054f, 0.800000011920929f};
	glm::vec4 NavWindowingHighlight = { 1.0f, 1.0f, 1.0f, 0.699999988079071f};
	glm::vec4 NavWindowingDimBg = { 0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f};
	glm::vec4 ModalWindowDimBg = { 0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f };
};

struct Theme {
	Style Style;
	Colors Colors;
};