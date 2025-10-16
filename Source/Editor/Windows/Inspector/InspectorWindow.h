#pragma once

#include "Core/Core.h"

#include "Editor/BaseWindow.h"

#include "Layers/Mandelbrot/FractalState.h"

#include <vector>

class InspectorWindow : public BaseWindow {
public:
	InspectorWindow(bool& isOpen, FractalState& fractalState);

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnUIRender() override;
private:
	FractalState& m_FractalState;

	std::vector<FractalAlgorithm> m_FractalAlgorithms;
	std::vector<ColorAlgorithm> m_ColorAlgorithms;
	std::vector<InteriorColorAlgorithm> m_InteriorColorAlgorithms;
	std::vector<OrbitTrapType> m_OrbitTrapTypes;
};