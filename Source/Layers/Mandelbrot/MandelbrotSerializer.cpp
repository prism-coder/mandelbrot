#include "MandelbrotSerializer.h"

#include "Core/Log.h"

#include "Utilities/Utilities.h"

#include <yaml-cpp/yaml.h>
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

	static Emitter& operator<<(Emitter& out, const glm::vec2& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << EndSeq;
		return out;
	}

	static Emitter& operator<<(Emitter& out, const glm::vec3& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << v.z << EndSeq;
		return out;
	}
}

MandelbrotSerializer::MandelbrotSerializer(Mandelbrot& mandelbrot)
	: m_Mandelbrot(mandelbrot)
{}

bool MandelbrotSerializer::Serialize(const std::filesystem::path& filepath) {
	Log::Trace("MandelbrotSerializer::Serialize - Serializing Mandelbrot settings to " + filepath.string());

	YAML::Emitter out;
	out << YAML::BeginMap; // Root
	{
		out << YAML::Key << "Mandelbrot" << YAML::Value << YAML::BeginMap; // Mandelbrot
		{
			out << YAML::Key << "FractalParameters" << YAML::Value << YAML::BeginMap; // FractalParameters
			{
				out << YAML::Key << "Algorithm" << YAML::Value << Utilities::FractalAlgorithmToString(m_Mandelbrot.Algorithm);
				out << YAML::Key << "Power" << YAML::Value << m_Mandelbrot.Power;
				out << YAML::Key << "Bailout" << YAML::Value << m_Mandelbrot.Bailout;
				out << YAML::Key << "MaxIterations" << YAML::Value << m_Mandelbrot.MaxIterations;
			}
			out << YAML::EndMap; // FractalParameters

			out << YAML::Key << "ViewParameters" << YAML::Value << YAML::BeginMap; // ViewParameters
			{
				out << YAML::Key << "Zoom" << YAML::Value << m_Mandelbrot.Zoom;
				out << YAML::Key << "Position" << YAML::Value << m_Mandelbrot.Position;
				out << YAML::Key << "Rotation" << YAML::Value << m_Mandelbrot.Rotation;
			}
			out << YAML::EndMap; // ViewParameters

			out << YAML::Key << "JuliaParameters" << YAML::Value << YAML::BeginMap; // JuliaParameters
			{
				out << YAML::Key << "JuliaMode" << YAML::Value << m_Mandelbrot.JuliaMode;
				out << YAML::Key << "JuliaC" << YAML::Value << m_Mandelbrot.JuliaC;
			}
			out << YAML::EndMap; // JuliaParameters

			out << YAML::Key << "ColoringParameters" << YAML::Value << YAML::BeginMap; // ColoringParameters
			{
				out << YAML::Key << "ExteriorColoring" << YAML::Value << Utilities::ColorAlgorithmToString(m_Mandelbrot.ExteriorColoring);
				out << YAML::Key << "InteriorColoring" << YAML::Value << Utilities::InteriorColorAlgorithmToString(m_Mandelbrot.InteriorColoring);
				out << YAML::Key << "InteriorColor" << YAML::Value << m_Mandelbrot.InteriorColor;
				out << YAML::Key << "ColorFrequency" << YAML::Value << m_Mandelbrot.ColorFrequency;
				out << YAML::Key << "ColorOffset" << YAML::Value << m_Mandelbrot.ColorOffset;
				out << YAML::Key << "OrbitColoring" << YAML::Value << m_Mandelbrot.OrbitColoring;
				out << YAML::Key << "DistanceScale" << YAML::Value << m_Mandelbrot.DistanceScale;

				out << YAML::Key << "ColorPalette" << YAML::Value << YAML::BeginMap; // ColorPalette
				{
					out << YAML::Key << "Colors" << YAML::Value << YAML::BeginSeq; // Colors
					for (const auto& color : m_Mandelbrot.ColorPalette.Colors) {
						out << YAML::Value << color;
					}
					out << YAML::EndSeq; // Colors
				}
				out << YAML::EndMap; // ColorPalette
			}
			out << YAML::EndMap; // ColoringParameters

			out << YAML::Key << "OrbitTrap" << YAML::Value << YAML::BeginMap; // OrbitTrap
			{
				out << YAML::Key << "Type" << YAML::Value << Utilities::OrbitTrapTypeToString(m_Mandelbrot.Trap.Type);
				out << YAML::Key << "P1" << YAML::Value << m_Mandelbrot.Trap.P1;
				out << YAML::Key << "P2" << YAML::Value << m_Mandelbrot.Trap.P2;
				out << YAML::Key << "Color" << YAML::Value << m_Mandelbrot.Trap.Color;
				out << YAML::Key << "Blend" << YAML::Value << m_Mandelbrot.Trap.Blend;
			}
			out << YAML::EndMap; // OrbitTrap
		}
		out << YAML::EndMap; // Mandelbrot
	}
	out << YAML::EndMap; // Root

	std::ofstream fout(filepath);
	if (!fout.is_open()) {
		Log::Error("MandelbrotSerializer::Serialize - Failed to open file for writing: " + filepath.string());
		return false;
	}

	fout << out.c_str();

	return true;
}

bool MandelbrotSerializer::Deserialize(const std::filesystem::path& filepath) {
	Log::Trace("MandelbrotSerializer::Deserialize - Deserializing Mandelbrot settings from " + filepath.string());

	YAML::Node data;

	try {
		data = YAML::LoadFile(filepath.string());
	} catch (YAML::Exception e) {
		Log::Error("MandelbrotSerializer::Deserialize - Failed to load file: " + std::string(e.what()));
		return false;
	}

	if (const auto& mandelbrotNode = data["Mandelbrot"]) {
		m_Mandelbrot.ColorPalette.Colors.clear();

		if (const auto& fractalParametersNode = mandelbrotNode["FractalParameters"]) {
			if (const auto& algorithmNode = fractalParametersNode["Algorithm"]) {
				m_Mandelbrot.Algorithm = Utilities::StringToFractalAlgorithm(algorithmNode.as<std::string>());
			}

			if (const auto& powerNode = fractalParametersNode["Power"]) {
				m_Mandelbrot.Power = powerNode.as<float>();
			}

			if (const auto& bailoutNode = fractalParametersNode["Bailout"]) {
				m_Mandelbrot.Bailout = bailoutNode.as<float>();
			}

			if (const auto& maxIterationsNode = fractalParametersNode["MaxIterations"]) {
				m_Mandelbrot.MaxIterations = maxIterationsNode.as<int>();
			}
		}

		if (const auto& viewParametersNode = mandelbrotNode["ViewParameters"]) {
			if (const auto& zoomNode = viewParametersNode["Zoom"]) {
				m_Mandelbrot.Zoom = zoomNode.as<float>();
			}

			if (const auto& positionNode = viewParametersNode["Position"]) {
				m_Mandelbrot.Position = positionNode.as<glm::vec2>();
			}

			if (const auto& rotationNode = viewParametersNode["Rotation"]) {
				m_Mandelbrot.Rotation = rotationNode.as<float>();
			}
		}

		if (const auto& juliaParametersNode = mandelbrotNode["JuliaParameters"]) {
			if (const auto& juliaModeNode = juliaParametersNode["JuliaMode"]) {
				m_Mandelbrot.JuliaMode = juliaModeNode.as<bool>();
			}

			if (const auto& juliaCNode = juliaParametersNode["JuliaC"]) {
				m_Mandelbrot.JuliaC = juliaCNode.as<glm::vec2>();
			}
		}

		if (const auto& coloringParametersNode = mandelbrotNode["ColoringParameters"]) {
			if (const auto& exteriorColoringNode = coloringParametersNode["ExteriorColoring"]) {
				m_Mandelbrot.ExteriorColoring = Utilities::StringToColorAlgorithm(exteriorColoringNode.as<std::string>());
			}

			if (const auto& interiorColoringNode = coloringParametersNode["InteriorColoring"]) {
				m_Mandelbrot.InteriorColoring = Utilities::StringToInteriorColorAlgorithm(interiorColoringNode.as<std::string>());
			}

			if (const auto& interiorColorNode = coloringParametersNode["InteriorColor"]) {
				m_Mandelbrot.InteriorColor = interiorColorNode.as<glm::vec3>();
			}

			if (const auto& colorFrequencyNode = coloringParametersNode["ColorFrequency"]) {
				m_Mandelbrot.ColorFrequency = colorFrequencyNode.as<float>();
			}

			if (const auto& colorOffsetNode = coloringParametersNode["ColorOffset"]) {
				m_Mandelbrot.ColorOffset = colorOffsetNode.as<float>();
			}

			if (const auto& orbitColoringNode = coloringParametersNode["OrbitColoring"]) {
				m_Mandelbrot.OrbitColoring = orbitColoringNode.as<bool>();
			}

			if (const auto& distanceScaleNode = coloringParametersNode["DistanceScale"]) {
				m_Mandelbrot.DistanceScale = distanceScaleNode.as<float>();
			}

			if (const auto& colorPaletteNode = coloringParametersNode["ColorPalette"]) {
				if (const auto& colorsNode = colorPaletteNode["Colors"]) {
					for (const auto& colorNode : colorsNode) {
						m_Mandelbrot.ColorPalette.Colors.push_back(colorNode.as<glm::vec3>());
					}
				}
			}
		}

		if (const auto& orbitTrapNode = mandelbrotNode["OrbitTrap"]) {
			if (const auto& typeNode = orbitTrapNode["Type"]) {
				m_Mandelbrot.Trap.Type = Utilities::StringToOrbitTrapType(typeNode.as<std::string>());
			}

			if (const auto& p1Node = orbitTrapNode["P1"]) {
				m_Mandelbrot.Trap.P1 = p1Node.as<glm::vec2>();
			}

			if (const auto& p2Node = orbitTrapNode["P2"]) {
				m_Mandelbrot.Trap.P2 = p2Node.as<glm::vec2>();
			}

			if (const auto& colorNode = orbitTrapNode["Color"]) {
				m_Mandelbrot.Trap.Color = colorNode.as<glm::vec3>();
			}

			if (const auto& blendNode = orbitTrapNode["Blend"]) {
				m_Mandelbrot.Trap.Blend = blendNode.as<float>();
			}
		}
		
		return true;
	}

	return false;
}