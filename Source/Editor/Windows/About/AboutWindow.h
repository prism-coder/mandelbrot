#pragma once

#include "Editor/BaseWindow.h"

class AboutWindow : public BaseWindow {
public:
	AboutWindow(bool& isOpen);

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnUIRender() override;
};