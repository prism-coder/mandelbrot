#include "ViewportWindow.h"

#include "Core/Application.h"
#include "Core/Input/Input.h"
#include "Core/Settings/SettingsManager.h"

#include "Renderer/Renderer.h"

#include "Editor/Payload.h"

#include "Layers/Mandelbrot/FractalState.h"

ViewportWindow::ViewportWindow(bool& isOpen, FractalState& fractalState)
	: BaseWindow(isOpen), m_FractalState(fractalState)
{}

void ViewportWindow::OnAttach() {
	Log::Trace("ViewportWindow::OnAttach - Attaching Viewport Window");
}

void ViewportWindow::OnDetach() {
	Log::Trace("ViewportWindow::OnDetach - Detaching Viewport Window");
}

void ViewportWindow::OnUpdate(Timestep ts) {
	if (!m_IsOpen || m_ViewportWidth == 0 || m_ViewportHeight == 0) {
		return;
	}

	if (m_ViewportHovered) {
		glm::vec2 moveDirection = { 0.0f, 0.0f };
		if (Input::IsKeyPressed(KeyCode::W)) moveDirection.y += 1.0f;
		if (Input::IsKeyPressed(KeyCode::S)) moveDirection.y -= 1.0f;
		if (Input::IsKeyPressed(KeyCode::A)) moveDirection.x -= 1.0f;
		if (Input::IsKeyPressed(KeyCode::D)) moveDirection.x += 1.0f;

		if (glm::length(moveDirection) > 0.0f) {
			moveDirection = glm::normalize(moveDirection);

			float rotationRadians = glm::radians(m_FractalState.Target.Rotation);

			glm::mat2 rotationMatrix = {
				{ cos(rotationRadians), -sin(rotationRadians) },
				{ sin(rotationRadians),  cos(rotationRadians) }
			};

			glm::vec2 rotatedMoveDirection = rotationMatrix * moveDirection;

			m_FractalState.Target.Position += rotatedMoveDirection * m_FractalState.MovementSpeed * (float)ts / m_FractalState.Target.Zoom;
		}

		if (Input::IsKeyPressed(KeyCode::LeftShift)) {
			m_FractalState.Target.Zoom += m_FractalState.ZoomSpeed * m_FractalState.Target.Zoom * ts;
		} else if (Input::IsKeyPressed(KeyCode::LeftControl)) {
			m_FractalState.Target.Zoom -= m_FractalState.ZoomSpeed * m_FractalState.Target.Zoom * ts;
		}

		if (Input::IsKeyPressed(KeyCode::Q)) {
			if (m_FractalState.Target.Rotation <= 0) {
				m_FractalState.Target.Rotation = 0;
			} else {
				m_FractalState.Target.Rotation -= m_FractalState.RotationSpeed * 100.0f * ts;
			}
		} else if (Input::IsKeyPressed(KeyCode::E)) {
			if (m_FractalState.Target.Rotation >= 360) {
				m_FractalState.Target.Rotation = 360;
			} else {
				m_FractalState.Target.Rotation += m_FractalState.RotationSpeed * 100.0f * ts;
			}
		}
	}

	Renderer::Begin();
	m_FractalState.Current.ColorPalette.PrepareForShader();
	Renderer::Submit(m_FractalState.Current);
	Renderer::End();
}

void ViewportWindow::OnUIRender() {
	if (!m_IsOpen) {
		return;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	if (ImGui::Begin("Viewport", &m_IsOpen)) {
		Update();
		Draw();
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void ViewportWindow::Update() {
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();

	const auto& resolution = SettingsManager::Get().Rendering.Resolution;

	m_ViewportWidth = (uint32_t)viewportSize.x * resolution.Scale;
	m_ViewportHeight = (uint32_t)viewportSize.y * resolution.Scale;

	Resize();

	m_ViewportHovered = ImGui::IsWindowHovered();
}

void ViewportWindow::Resize() {
	if (m_ViewportWidth > 0 && m_ViewportHeight > 0) {
		// 1. Resize the framebuffer
		if (auto framebuffer = Renderer::GetFramebuffer()) {
			if (auto colorAttachment = framebuffer->GetColorAttachment()) {
				if (colorAttachment->GetWidth() != m_ViewportWidth ||
					colorAttachment->GetHeight() != m_ViewportHeight
				) {
					framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);
				}
			}
		}
	}
}

void ViewportWindow::Draw() {
	Ref<Texture2D> finalImage = Renderer::GetFramebuffer()->GetColorAttachment();

	if (finalImage) {
		const auto& resolution = SettingsManager::Get().Rendering.Resolution;

		ImGui::Image(
			(ImTextureRef)finalImage->GetHandle(),
			{ (float)finalImage->GetWidth() / resolution.Scale, (float)finalImage->GetHeight() / resolution.Scale },
			{ 0, 1 }, { 1, 0 }
		);
	}

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Payload::Configuration)) {
			const char* path = (const char*)payload->Data;

			if (m_ConfigurationLoadCallback) {
				m_ConfigurationLoadCallback(std::filesystem::path(path));
			}
		}

		ImGui::EndDragDropTarget();
	}
}
