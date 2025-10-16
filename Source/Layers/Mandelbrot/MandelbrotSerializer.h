#pragma once

#pragma once

#include "Layers/Mandelbrot/Mandelbrot.h"

#include <filesystem>

class MandelbrotSerializer {
public:
	MandelbrotSerializer(Mandelbrot& mandelbrot);

	bool Serialize(const std::filesystem::path& filepath);
	bool Deserialize(const std::filesystem::path& filepath);
private:
	Mandelbrot& m_Mandelbrot;
};