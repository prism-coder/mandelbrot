#pragma once

#include "Core/Settings/Settings.h"

#include "Layers/Mandelbrot/Mandelbrot.h"
#include "Layers/Mandelbrot/OrbitTrap.h"

#include "Editor/Theme/Theme.h"

#include <string>

class Utilities {
public:
	static std::string RenderingEngineToString(const RenderingEngine& engine);
	static RenderingEngine StringToRenderingEngine(const std::string& engine);

	static std::string EditorThemeToString(const EditorTheme& theme);
	static EditorTheme StringToEditorTheme(const std::string& theme);

	static std::string CategoryToString(const Category& category);
	static Category StringToCategory(const std::string& category);

	static std::string ColorAlgorithmToString(const ColorAlgorithm& algorithm);
	static ColorAlgorithm StringToColorAlgorithm(const std::string& algorithm);

	static std::string InteriorColorAlgorithmToString(const InteriorColorAlgorithm& algorithm);
	static InteriorColorAlgorithm StringToInteriorColorAlgorithm(const std::string& algorithm);

	static std::string OrbitTrapTypeToString(const OrbitTrapType& type);
	static OrbitTrapType StringToOrbitTrapType(const std::string& type);

	static std::string FractalAlgorithmToString(const FractalAlgorithm& algorithm);
	static FractalAlgorithm StringToFractalAlgorithm(const std::string& algorithm);

	static std::string DirectionToString(const Direction& direction);
	static Direction StringToDirection(const std::string& direction);
};