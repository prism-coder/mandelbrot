#pragma once

#include "Editor/BaseWindow.h"

#include <filesystem>
#include <functional>

struct FractalState;

class ViewportWindow : public BaseWindow {
public:
	using ConfigurationLoadCallback = std::function<void(const std::filesystem::path&)>;

	ViewportWindow(bool& isOpen, FractalState& fractalState);

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnUIRender() override;

	void SetConfigurationLoadCallback(const ConfigurationLoadCallback& callback) { m_ConfigurationLoadCallback = callback; }
private:
	void Update();
	void Resize();
	void Draw();
private:
	bool m_ViewportHovered = false;

	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;

	FractalState& m_FractalState;

	ConfigurationLoadCallback m_ConfigurationLoadCallback;
};