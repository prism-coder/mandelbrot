#pragma once

#include "Core/Timestep.h"

class Layer {
public:
	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(Timestep ts) {}
	virtual void OnUIRender() {}
};