#pragma once

#include "Core/Timestep.h"

#include "Mandelbrot.h"

#include <glm/gtc/constants.hpp>

#include <glm/gtx/compatibility.hpp>

struct FractalState {
	// The state being rendered
	Mandelbrot Current;

	// The state that checks and file uploads modify
	Mandelbrot Target;

	// Smoothing factor. A higher value = faster transition.
	float Smoothing = 5.0f;

	// Speed for movement and zoom. A higher value = faster movement.
	float MovementSpeed = 1.0f;
	float RotationSpeed = 1.0f;
	float ZoomSpeed = 1.0f;

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