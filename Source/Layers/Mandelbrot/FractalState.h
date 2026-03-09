#pragma once

#include "Core/Timestep.h"

#include "Mandelbrot.h"

#include <glm/gtc/constants.hpp>

#include <glm/gtx/compatibility.hpp>

/**
 * This struct holds the current state of the fractal being rendered, as well as the target state that is modified by user input and file uploads.
 */
struct FractalState {
	/// @brief The current state of the fractal parameters that is being rendered.
	Mandelbrot Current;

	/// @brief The target state of the fractal parameters that is modified by user input and file uploads.
	Mandelbrot Target;

	/// @brief The smoothing factor for parameter interpolation. Higher values result in faster transitions.
	float Smoothing = 5.0f;

	// Speed for movement, rotation, zoom and power. A higher value = faster movement.
	float MovementSpeed = 2.0f;
	float RotationSpeed = 2.0f;
	float ZoomSpeed = 2.0f;
	float PowerSpeed = 2.0f;

	/**
	 * Updates the current state by interpolating towards the target state based on the elapsed time (Timestep).
	 * This method should be called every frame with the time since the last frame to ensure smooth transitions between parameter changes.
	 * 
	 * @param ts The time elapsed since the last update, used to calculate the interpolation factor for smooth transitions.
	 */
	void Update(Timestep ts) {
		// This is the formula for exponential interpolation, which is independent of the framerate
		// and gives a very pleasant "acceleration and braking" feel.
		float alpha = 1.0f - exp(-Smoothing * ts);

		// We interpolate each of the parameters
		Current.Power = glm::lerp(Current.Power, Target.Power, alpha);
		Current.Bailout = glm::lerp(Current.Bailout, Target.Bailout, alpha);
		Current.Zoom = glm::lerp(Current.Zoom, Target.Zoom, alpha);
		Current.Position = glm::mix(Current.Position, Target.Position, alpha);
		Current.Rotation = glm::lerp(Current.Rotation, Target.Rotation, alpha);
		Current.JuliaC = glm::mix(Current.JuliaC, Target.JuliaC, alpha);

		Current.ColorFrequency = glm::lerp(Current.ColorFrequency, Target.ColorFrequency, alpha);
		Current.ColorOffset = glm::lerp(Current.ColorOffset, Target.ColorOffset, alpha);
		Current.DistanceScale = glm::lerp(Current.DistanceScale, Target.DistanceScale, alpha);
		Current.InteriorColor = glm::mix(Current.InteriorColor, Target.InteriorColor, alpha);

		Current.Trap.P1 = glm::mix(Current.Trap.P1, Target.Trap.P1, alpha);
		Current.Trap.P2 = glm::mix(Current.Trap.P2, Target.Trap.P2, alpha);
		Current.Trap.Color = glm::mix(Current.Trap.Color, Target.Trap.Color, alpha);
		Current.Trap.Blend = glm::lerp(Current.Trap.Blend, Target.Trap.Blend, alpha);

		// Discrete values ​​(enums, bools) are changed instantly
		// to avoid strange behavior during the transition.
		Current.Algorithm = Target.Algorithm;
		Current.OrbitColoring = Target.OrbitColoring;
		Current.JuliaMode = Target.JuliaMode;
		Current.MaxIterations = Target.MaxIterations;
		Current.ExteriorColoring = Target.ExteriorColoring;
		Current.InteriorColoring = Target.InteriorColoring;
		Current.ColorPalette = Target.ColorPalette;
		Current.Trap.Type = Target.Trap.Type;
	}
};