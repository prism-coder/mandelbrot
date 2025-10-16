#pragma once

#include "Core/Timestep.h"

#include "imgui.h"

class BaseWindow {
public:
	BaseWindow(bool& isOpen) : m_IsOpen(isOpen) {}
	virtual ~BaseWindow() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(Timestep ts) {}
	virtual void OnUIRender() = 0;

	void Close() { m_IsOpen = false; }
protected:
	bool& m_IsOpen;
};