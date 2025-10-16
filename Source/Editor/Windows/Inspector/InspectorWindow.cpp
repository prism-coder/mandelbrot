#include "InspectorWindow.h"

#include "Core/Log.h"

#include "Editor/UI.h"

#include "Utilities/Utilities.h"

InspectorWindow::InspectorWindow(bool& isOpen, FractalState& fractalState)
	: BaseWindow(isOpen), m_FractalState(fractalState)
{}

void InspectorWindow::OnAttach() {
	Log::Trace("InspectorWindow::OnAttach - Attaching Inspector Window");

	m_FractalAlgorithms = {
		FractalAlgorithm::Mandelbrot,
		FractalAlgorithm::BurningShip,
		FractalAlgorithm::Tricorn
	};

	m_ColorAlgorithms = {
		ColorAlgorithm::Step,
		ColorAlgorithm::Smooth,
		ColorAlgorithm::DistanceEstimation
	};

	m_InteriorColorAlgorithms = {
		InteriorColorAlgorithm::Black,
		InteriorColorAlgorithm::White,
		InteriorColorAlgorithm::CustomColor
	};

	m_OrbitTrapTypes = {
		OrbitTrapType::None,
		OrbitTrapType::Point,
		OrbitTrapType::Circle,
		OrbitTrapType::Line,
		OrbitTrapType::Box,
		OrbitTrapType::Cross
	};
}

void InspectorWindow::OnDetach() {
	Log::Trace("InspectorWindow::OnDetach - Detaching Inspector Window");
}

void InspectorWindow::OnUpdate(Timestep ts) {}

void InspectorWindow::OnUIRender() {
	if (!m_IsOpen) {
		return;
	}

	Mandelbrot& mandelbrot = m_FractalState.Target;

	if (ImGui::Begin("Inspector", &m_IsOpen)) {
		if (UI::CollapsingHeader("Animation")) {
			UI::SliderFloat("Smoothing Factor", m_FractalState.Smoothing, 0.1f, 20.0f);
			UI::Tooltip("Controls the speed of transitions.\nHigher is faster.");

			UI::SliderFloat("Movement Speed", m_FractalState.MovementSpeed, 0.01f, 10.0f);
			UI::Tooltip("Controls the speed of the movement.\nHigher is faster.");

			UI::SliderFloat("Rotation Speed", m_FractalState.RotationSpeed, 0.01f, 10.0f);
			UI::Tooltip("Controls the speed of the rotation.\nHigher is faster.");

			UI::SliderFloat("Zoom Speed", m_FractalState.ZoomSpeed, 0.01f, 10.0f);
			UI::Tooltip("Controls the speed of the zoom.\nHigher is faster.");

			UI::Separator();
		}

		if (UI::CollapsingHeader("Fractal")) {
			UI::Dropdown("Algorithm", m_FractalAlgorithms, mandelbrot.Algorithm, Utilities::FractalAlgorithmToString);
			UI::Tooltip("Changes the core mathematical formula used to generate the fractal.");

			UI::DragFloat("Power", mandelbrot.Power, 0.01f, 10.0f, 0.01f);
			UI::Tooltip("The exponent 'n' in the formula z = z^n + c.\nCreates different fractal shapes (Multibrot sets).");

			UI::SliderInt("Max Iterations", mandelbrot.MaxIterations, 32, 8192);
			UI::Tooltip("The maximum number of calculations per pixel.\nHigher values reveal more detail but are slower.");

			UI::DragFloat("Bailout Radius", mandelbrot.Bailout, 4.0f, 65536.0f);
			UI::Tooltip("The escape boundary.\nIf a point's magnitude exceeds this value, it has escaped.\nAffects edge detail.");

			UI::Bool("Julia Mode", mandelbrot.JuliaMode);
			UI::Tooltip("Switches to a Julia set calculation, where 'c' is a constant for the entire image.");

			if (mandelbrot.JuliaMode) {
				UI::Separator();

				UI::Vec2("Constant 'c'", mandelbrot.JuliaC, 0.0f);
				UI::Tooltip("The constant 'c' value for the Julia set calculation.\nThe entire fractal is based on this point.");
			}

			UI::Separator();
		}

		if (UI::CollapsingHeader("View")) {
			UI::DragFloat("Zoom", mandelbrot.Zoom, 0.01f, 10000.0f, 0.1f);
			UI::Tooltip("Magnification level of the fractal.");

			{
				glm::vec2 positionBeforeDrag = mandelbrot.Position;
				glm::vec2 positionAfterDrag = positionBeforeDrag;

				UI::Vec2("Position", positionAfterDrag);
				UI::Tooltip("Pans the view across the complex plane.");

				if (positionAfterDrag != positionBeforeDrag) {
					glm::vec2 delta = positionAfterDrag - positionBeforeDrag;

					float inverseRotation = glm::radians(-mandelbrot.Rotation);

					glm::mat2 rotMat = {
						{ cos(inverseRotation), -sin(inverseRotation) },
						{ sin(inverseRotation),  cos(inverseRotation) }
					};

					glm::vec2 screenSpaceDelta = rotMat * delta;

					mandelbrot.Position = positionBeforeDrag + screenSpaceDelta;
				}
			}

			UI::SliderFloat("Rotation", mandelbrot.Rotation, 0.0f, 360.0f);
			UI::Tooltip("Rotates the view around the current position (in degrees).");

			UI::Separator();
		}

		if (UI::CollapsingHeader("Coloring")) {
			UI::Dropdown("Exterior", m_ColorAlgorithms, mandelbrot.ExteriorColoring, Utilities::ColorAlgorithmToString);
			UI::Tooltip("Algorithm for coloring the area outside the set.");

			UI::Dropdown("Interior", m_InteriorColorAlgorithms, mandelbrot.InteriorColoring, Utilities::InteriorColorAlgorithmToString);
			UI::Tooltip("Algorithm for coloring the area inside the set.");

			if (mandelbrot.ExteriorColoring == ColorAlgorithm::DistanceEstimation) {
				UI::DragFloat("Distance Scale", mandelbrot.DistanceScale, 1.0f, 500.0f);
				UI::Tooltip("Controls the base thickness and spacing of the contour lines in Distance Estimation mode.");
			}

			if (mandelbrot.InteriorColoring == InteriorColorAlgorithm::CustomColor) {
				UI::Separator();

				UI::ColorEdit3("Interior Color", mandelbrot.InteriorColor);
				UI::Tooltip("The custom color for the interior of the set when 'CustomColor' mode is selected.");
			}

			UI::DragFloat("Color Frequency", mandelbrot.ColorFrequency, 0.01f, 100.0f);
			UI::Tooltip("How many times the color palette gradient repeats.\nHigher values create more color bands.");

			UI::DragFloat("Color Phase Shift", mandelbrot.ColorOffset, 0.001f, 1.0f);
			UI::Tooltip("Shifts the starting point of the color palette, cycling the colors.");

			UI::Bool("Use Orbit Position", mandelbrot.OrbitColoring);
			UI::Tooltip("Blends the color based on the final position (angle) of the point before escaping.");


			UI::Separator();

			UI::List("Color Palette", mandelbrot.ColorPalette.Colors, [&](glm::vec3& color) {
				UI::ColorEdit3(color);
				UI::Tooltip("Color segment of the Palette\nThe maximum number of colors is 16.");
			});

			UI::Separator();
		}

		if (UI::CollapsingHeader("Orbit Trap")) {
			UI::Dropdown("Type", m_OrbitTrapTypes, mandelbrot.Trap.Type, Utilities::OrbitTrapTypeToString);
			UI::Tooltip("Selects the shape of the orbit trap, which influences the fractal's color.");

			if (mandelbrot.Trap.Type != OrbitTrapType::None) {
				if (mandelbrot.Trap.Type == OrbitTrapType::Point) { // Point
					UI::Vec2("Position", mandelbrot.Trap.P1, 0.0f);
					UI::Tooltip("The center of the point trap in the complex plane.");
				} else if (mandelbrot.Trap.Type == OrbitTrapType::Circle) { // Circle
					UI::Vec2("Center", mandelbrot.Trap.P1, 0.0f);
					UI::Tooltip("The center of the circle trap.");

					UI::DragFloat("Radius", mandelbrot.Trap.P2.x, 0.001f, 10.0f);
					UI::Tooltip("The radius of the circle trap.");
				} else if (mandelbrot.Trap.Type == OrbitTrapType::Line) { // Line
					UI::Vec2("Start Point", mandelbrot.Trap.P1, 0.0f);
					UI::Tooltip("The starting point of the line segment trap.");

					UI::Vec2("End Point", mandelbrot.Trap.P2, 0.0f);
					UI::Tooltip("The ending point of the line segment trap.");
				} else if (mandelbrot.Trap.Type == OrbitTrapType::Box) { // Box
					UI::Vec2("Center", mandelbrot.Trap.P1, 0.0f);
					UI::Tooltip("The center of the box trap.");

					UI::Vec2("Size", mandelbrot.Trap.P2, 0.5f);
					UI::Tooltip("The width and height of the box trap.");
				} else if (mandelbrot.Trap.Type == OrbitTrapType::Cross) { // Cross
					UI::Vec2("Center", mandelbrot.Trap.P1, 0.0f);
					UI::Tooltip("The center of the cross trap.");
				}

				UI::Separator();

				UI::ColorEdit3("Trap Color", mandelbrot.Trap.Color);
				UI::Tooltip("The color that will be blended into the fractal near the trap.");

				UI::SliderFloat("Blend Factor", mandelbrot.Trap.Blend, 0.0f, 1.0f);
				UI::Tooltip("How strongly the trap's color is blended with the fractal's original color.");
			}

			UI::Separator();
		}
	}

	ImGui::End();
}