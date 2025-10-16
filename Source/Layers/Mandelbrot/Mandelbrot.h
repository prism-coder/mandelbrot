#pragma once

#include "Palette.h"
#include "OrbitTrap.h"

#include <glm/glm.hpp>

enum class FractalAlgorithm {
	Mandelbrot,
	BurningShip,
	Tricorn
};

enum class ColorAlgorithm {
	Step,
	Smooth,
	DistanceEstimation
};

enum class InteriorColorAlgorithm {
	Black,
	White,
	CustomColor
};

struct Mandelbrot {
	// Fractal Parameters
	FractalAlgorithm Algorithm = FractalAlgorithm::Mandelbrot;
	float Power = 2.0f; // Power 'n' in z = z^n + c
	float Bailout = 16.0f; // Bailout radius (larger = more detail on the edges)
	int MaxIterations = 256;

	// View Parameters
	float Zoom = 1.0f;
	glm::vec2 Position = { -0.5f, 0.0f };
	float Rotation = 0.0f;

	// Julia Parameters
	bool JuliaMode = false;
	glm::vec2 JuliaC = { -0.8f, 0.156f }; // Constant 'c' coordinate for Julia

	// Coloring Parameters
	ColorAlgorithm ExteriorColoring = ColorAlgorithm::Smooth;
	InteriorColorAlgorithm InteriorColoring = InteriorColorAlgorithm::CustomColor;
	glm::vec3 InteriorColor = { 0.0f, 0.0f, 0.0f };
	float ColorFrequency = 1.0f;
	float ColorOffset = 0.0f;
	bool OrbitColoring = false;
	float DistanceScale = 50.0f;
	Palette ColorPalette;

	// Orbit Trap
	OrbitTrap Trap;
};