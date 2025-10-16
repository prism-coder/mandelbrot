#pragma once

#include "Editor/BaseWindow.h"

class StatisticsWindow : public BaseWindow {
public:
	StatisticsWindow(bool& isOpen);

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnUIRender() override;
};