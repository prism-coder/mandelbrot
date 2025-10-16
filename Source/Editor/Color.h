#pragma once

#include "imgui.h"

struct Color {
	const inline static ImVec4 Red = { 0.8f, 0.2f, 0.2f, 1.0f };
	const inline static ImVec4 Green = { 0.2f, 0.8f, 0.2f, 1.0f };
	const inline static ImVec4 Blue = { 0.2f, 0.2f, 0.8f, 1.0f };
	const inline static ImVec4 Magenta = { 0.8f, 0.2f, 0.8f, 1.0f };
	const inline static ImVec4 Yellow = { 0.8f, 0.8f, 0.2f, 1.0f };
	const inline static ImVec4 Orange = { 0.8f, 0.5f, 0.2f, 1.0f };
	const inline static ImVec4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	const inline static ImVec4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	const inline static ImVec4 Transparent = { 0.0f, 0.0f, 0.0f, 0.0f };
};