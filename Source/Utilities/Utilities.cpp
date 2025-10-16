#include "Utilities.h"

#include "Core/Log.h"

std::string Utilities::RenderingEngineToString(const RenderingEngine& engine) {
	switch (engine) {
		case RenderingEngine::OpenGL:	return "OpenGL";
		case RenderingEngine::DirectX:	return "DirectX";
		case RenderingEngine::Vulkan:	return "Vulkan";
		default:						return "Unknown";
	}
}

RenderingEngine Utilities::StringToRenderingEngine(const std::string& engine) {
	if (engine == "OpenGL")		return RenderingEngine::OpenGL;
	if (engine == "DirectX")	return RenderingEngine::DirectX;
	if (engine == "Vulkan")		return RenderingEngine::Vulkan;

	Log::Error("Utilities::StringToRenderingEngine - Unknown Rendering Engine");

	return RenderingEngine::OpenGL;
}

std::string Utilities::EditorThemeToString(const EditorTheme& theme) {
	switch (theme) {
		case EditorTheme::AdobeInspired:			return "Adobe Inspired";
		case EditorTheme::AyuDark:					return "Ayu Dark";
		case EditorTheme::BessDark:					return "Bess Dark";
		case EditorTheme::BlackDevil:				return "Black Devil";
		case EditorTheme::BootstrapDark:			return "Bootstrap Dark";
		case EditorTheme::Carbon:					return "Carbon";
		case EditorTheme::Cherno:					return "Cherno";
		case EditorTheme::Cherry:					return "Cherry";
		case EditorTheme::ClassicSteam:				return "Classic Steam";
		case EditorTheme::Classic:					return "Classic";
		case EditorTheme::ComfortableDarkCyan:		return "Comfortable Dark Cyan";
		case EditorTheme::ComfortableLightOrange:	return "Comfortable Light Orange";
		case EditorTheme::Comfy:					return "Comfy";
		case EditorTheme::Darcula:					return "Darcula";
		case EditorTheme::DarkRed:					return "Dark Red";
		case EditorTheme::DarkRuda:					return "Dark Ruda";
		case EditorTheme::Dark:						return "Dark";
		case EditorTheme::Darky:					return "Darky";
		case EditorTheme::DeepDark:					return "Deep Dark";
		case EditorTheme::DiscordDark:				return "Discord Dark";
		case EditorTheme::Enemymouse:				return "Enemymouse";
		case EditorTheme::Everforest:				return "Everforest";
		case EditorTheme::Excellency:				return "Excellency";
		case EditorTheme::FutureDark:				return "Future Dark";
		case EditorTheme::Gold:						return "Gold";
		case EditorTheme::GreenFont:				return "Green Font";
		case EditorTheme::GreenLeaf:				return "Green Leaf";
		case EditorTheme::HazyDark:					return "Hazy Dark";
		case EditorTheme::LedSynthmaster:			return "Led Synthmaster";
		case EditorTheme::Light:					return "Light";
		case EditorTheme::MaterialFlat:				return "Material Flat";
		case EditorTheme::Microsoft:				return "Microsoft";
		case EditorTheme::Modern:					return "Modern";
		case EditorTheme::Photoshop:				return "Photoshop";
		case EditorTheme::PurpleComfy:				return "Purple Comfy";
		case EditorTheme::QuickMinimalLook:			return "Quick Minimal Look";
		case EditorTheme::RedFont:					return "Red Font";
		case EditorTheme::RedOni:					return "Red Oni";
		case EditorTheme::Rest:						return "Rest";
		case EditorTheme::RoundedVisualStudio:		return "Rounded Visual Studio";
		case EditorTheme::SoftCherry:				return "Soft Cherry";
		case EditorTheme::SonicRiders:				return "Sonic Riders";
		case EditorTheme::Unreal:					return "Unreal";
		case EditorTheme::VisualStudio:				return "Visual Studio";
		case EditorTheme::Windark:					return "Windark";
		default:									return "Unknown";
	}
}

EditorTheme Utilities::StringToEditorTheme(const std::string& theme) {
	if (theme == "Adobe Inspired")				return EditorTheme::AdobeInspired;
	if (theme == "Ayu Dark")					return EditorTheme::AyuDark;
	if (theme == "Bess Dark")					return EditorTheme::BessDark;
	if (theme == "Black Devil")					return EditorTheme::BlackDevil;
	if (theme == "Bootstrap Dark")				return EditorTheme::BootstrapDark;
	if (theme == "Carbon")						return EditorTheme::Carbon;
	if (theme == "Cherno")						return EditorTheme::Cherno;
	if (theme == "Cherry")						return EditorTheme::Cherry;
	if (theme == "Classic Steam")				return EditorTheme::ClassicSteam;
	if (theme == "Classic")						return EditorTheme::Classic;
	if (theme == "Comfortable Dark Cyan")		return EditorTheme::ComfortableDarkCyan;
	if (theme == "Comfortable Light Orange")	return EditorTheme::ComfortableLightOrange;
	if (theme == "Comfy")						return EditorTheme::Comfy;
	if (theme == "Darcula")						return EditorTheme::Darcula;
	if (theme == "Dark Red")					return EditorTheme::DarkRed;
	if (theme == "Dark Ruda")					return EditorTheme::DarkRuda;
	if (theme == "Dark")						return EditorTheme::Dark;
	if (theme == "Darky")						return EditorTheme::Darky;
	if (theme == "Deep Dark")					return EditorTheme::DeepDark;
	if (theme == "Discord Dark")				return EditorTheme::DiscordDark;
	if (theme == "Enemymouse")					return EditorTheme::Enemymouse;
	if (theme == "Everforest")					return EditorTheme::Everforest;
	if (theme == "Excellency")					return EditorTheme::Excellency;
	if (theme == "Future Dark")					return EditorTheme::FutureDark;
	if (theme == "Gold")						return EditorTheme::Gold;
	if (theme == "Green Font")					return EditorTheme::GreenFont;
	if (theme == "Green Leaf")					return EditorTheme::GreenLeaf;
	if (theme == "Hazy Dark")					return EditorTheme::HazyDark;
	if (theme == "Led Synthmaster")				return EditorTheme::LedSynthmaster;
	if (theme == "Light")						return EditorTheme::Light;
	if (theme == "Material Flat")				return EditorTheme::MaterialFlat;
	if (theme == "Microsoft")					return EditorTheme::Microsoft;
	if (theme == "Modern")						return EditorTheme::Modern;
	if (theme == "Photoshop")					return EditorTheme::Photoshop;
	if (theme == "Purple Comfy")				return EditorTheme::PurpleComfy;
	if (theme == "Quick Minimal Look")			return EditorTheme::QuickMinimalLook;
	if (theme == "Red Font")					return EditorTheme::RedFont;
	if (theme == "Red Oni")						return EditorTheme::RedOni;
	if (theme == "Rest")						return EditorTheme::Rest;
	if (theme == "Rounded Visual Studio")		return EditorTheme::RoundedVisualStudio;
	if (theme == "Soft Cherry")					return EditorTheme::SoftCherry;
	if (theme == "Sonic Riders")				return EditorTheme::SonicRiders;
	if (theme == "Unreal")						return EditorTheme::Unreal;
	if (theme == "Visual Studio")				return EditorTheme::VisualStudio;
	if (theme == "Windark")						return EditorTheme::Windark;

	Log::Error("Utilities::StringToEditorTheme - Unknown Editor Theme");

	return EditorTheme::Excellency;
}

std::string Utilities::CategoryToString(const Category& category) {
	switch (category) {
		case Category::Application:		return "Application";
		case Category::Editor:			return "Editor";
		case Category::Graphics:		return "Graphics";
		case Category::Input:			return "Input";
		case Category::Localization:	return "Localization";
		case Category::Quality:			return "Quality";
		case Category::Rendering:		return "Rendering";
		case Category::Time:			return "Time";
		default:						return "Unknown";
	}
}

Category Utilities::StringToCategory(const std::string& category) {
	if (category == "Application")	return Category::Application;
	if (category == "Editor")		return Category::Editor;
	if (category == "Graphics")		return Category::Graphics;
	if (category == "Input")		return Category::Input;
	if (category == "Localization")	return Category::Localization;
	if (category == "Quality")		return Category::Quality;
	if (category == "Rendering")	return Category::Rendering;
	if (category == "Time")			return Category::Time;

	Log::Error("Utilities::StringToCategory - Unknown Category");

	return Category::Application;
}

std::string Utilities::ColorAlgorithmToString(const ColorAlgorithm& algorithm) {
	switch (algorithm) {
		case ColorAlgorithm::Step:					return "Step";
		case ColorAlgorithm::Smooth:				return "Smooth";
		case ColorAlgorithm::DistanceEstimation:	return "Distance Estimation";
		default:									return "Unknown";
	}
}

ColorAlgorithm Utilities::StringToColorAlgorithm(const std::string& algorithm) {
	if (algorithm == "Step")				return ColorAlgorithm::Step;
	if (algorithm == "Smooth")				return ColorAlgorithm::Smooth;
	if (algorithm == "Distance Estimation")	return ColorAlgorithm::DistanceEstimation;

	Log::Error("Utilities::StringToColorAlgorithm - Unknown Color Algorithm");

	return ColorAlgorithm::Step;
}

std::string Utilities::InteriorColorAlgorithmToString(const InteriorColorAlgorithm& algorithm) {
	switch (algorithm) {
		case InteriorColorAlgorithm::Black:			return "Black";
		case InteriorColorAlgorithm::White:			return "White";
		case InteriorColorAlgorithm::CustomColor:	return "Custom Color";
		default:									return "Unknown";
	}
}

InteriorColorAlgorithm Utilities::StringToInteriorColorAlgorithm(const std::string& algorithm) {
	if (algorithm == "Black")			return InteriorColorAlgorithm::Black;
	if (algorithm == "White")			return InteriorColorAlgorithm::White;
	if (algorithm == "Custom Color")	return InteriorColorAlgorithm::CustomColor;

	Log::Error("Utilities::StringToInteriorColorAlgorithm - Unknown Interior Color Algorithm");

	return InteriorColorAlgorithm::Black;
}

std::string Utilities::OrbitTrapTypeToString(const OrbitTrapType& type) {
	switch (type) {
		case OrbitTrapType::None:	return "None";
		case OrbitTrapType::Point:	return "Point";
		case OrbitTrapType::Circle:	return "Circle";
		case OrbitTrapType::Line:	return "Line";
		case OrbitTrapType::Box:	return "Box";
		case OrbitTrapType::Cross:	return "Cross";
		default:					return "Unknown";
	}
}

OrbitTrapType Utilities::StringToOrbitTrapType(const std::string& type) {
	if (type == "None")		return OrbitTrapType::None;
	if (type == "Point")	return OrbitTrapType::Point;
	if (type == "Circle")	return OrbitTrapType::Circle;
	if (type == "Line")		return OrbitTrapType::Line;
	if (type == "Box")		return OrbitTrapType::Box;
	if (type == "Cross")	return OrbitTrapType::Cross;

	Log::Error("Utilities::StringToOrbitTrapType - Unknown Orbit Trap Type");

	return OrbitTrapType::None;
}

std::string Utilities::FractalAlgorithmToString(const FractalAlgorithm& algorithm) {
	switch (algorithm) {
		case FractalAlgorithm::Mandelbrot:	return "Mandelbrot";
		case FractalAlgorithm::BurningShip:	return "Burning Ship";
		case FractalAlgorithm::Tricorn:		return "Tricorn";
		default:							return "Unknown";
	}
}

FractalAlgorithm Utilities::StringToFractalAlgorithm(const std::string& algorithm) {
	if (algorithm == "Mandelbrot")	return FractalAlgorithm::Mandelbrot;
	if (algorithm == "Burning Ship")	return FractalAlgorithm::BurningShip;
	if (algorithm == "Tricorn")		return FractalAlgorithm::Tricorn;

	Log::Error("Utilities::StringToFractalAlgorithm - Unknown Fractal Algorithm");

	return FractalAlgorithm::Mandelbrot;
}

std::string Utilities::DirectionToString(const Direction& direction) {
	switch (direction) {
		case Direction::None:	return "None";
		case Direction::Left:	return "Left";
		case Direction::Right:	return "Right";
		case Direction::Up:		return "Up";
		case Direction::Down:	return "Down";
		default:				return "Unknown";
	}
}

Direction Utilities::StringToDirection(const std::string& direction) {
	if (direction == "None")	return Direction::None;
	if (direction == "Left")	return Direction::Left;
	if (direction == "Right")	return Direction::Right;
	if (direction == "Up")		return Direction::Up;
	if (direction == "Down")	return Direction::Down;

	Log::Error("Utilities::StringToDirection - Unknown Direction");

	return Direction::Left;
}
