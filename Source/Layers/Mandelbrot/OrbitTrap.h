#pragma once

#include <glm/glm.hpp>

enum class OrbitTrapType {
	None,
	Point,
	Circle,
	Line,
	Box,
	Cross
};

struct OrbitTrap {
	OrbitTrapType Type = OrbitTrapType::None;

	// Used for: Point (position), Circle (center), Line (start point)
	glm::vec2 P1 = { 0.0f, 0.0f };

	// Used for: Circle (radius at x), Line (end point)
	glm::vec2 P2 = { 0.5f, 0.5f };

	glm::vec3 Color = { 1.0f, 1.0f, 0.0f }; // Trap color
	float Blend = 0.5f; // Blending factor with the fractal color
};