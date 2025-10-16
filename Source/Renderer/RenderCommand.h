#pragma once

#include "Core/Log.h"

#include "Renderer/RendererAPI.h"

class RenderCommand {
public:
	static void Init() {
		Log::Trace("RenderCommand::Init - Initializing Render Command");
		s_RendererAPI->Init();
	}

	static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		s_RendererAPI->SetViewport(x, y, width, height);
	}

	static void SetClearColor(const glm::vec4& color) {
		s_RendererAPI->SetClearColor(color);
	}

	static void Clear() {
		s_RendererAPI->Clear();
	}

	static void ClearDepth() {
		s_RendererAPI->ClearDepth();
	}
	
	static void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) {
		s_RendererAPI->DrawArrays(vertexArray, vertexCount);
	}

	static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) {
		s_RendererAPI->DrawIndexed(vertexArray, indexCount);
	}

	static void DispatchCompute(uint32_t groupX, uint32_t groupY, uint32_t groupZ) {
		s_RendererAPI->DispatchCompute(groupX, groupY, groupZ);
	}

	static void BlitFramebufferToSwapchain(uint32_t fbo, uint32_t width, uint32_t height) {
		s_RendererAPI->BlitFramebufferToSwapchain(fbo, width, height);
	}

	static void EnableDepthTest(const bool& enable) {
		s_RendererAPI->EnableDepthTest(enable);
	}

	static void EnableDepthMask(const bool& enable) {
		s_RendererAPI->EnableDepthMask(enable);
	}

	static void EnableCullFace(const bool& enable) {
		s_RendererAPI->EnableCullFace(enable);
	}

	static void SetDepthFunction(const DepthFunction& depthFunction) {
		s_RendererAPI->SetDepthFunction(depthFunction);
	}

	static void SubmitTextureForDeletion(uint32_t handle) {
		s_RendererAPI->QueueTextureForDeletion(handle);
	}

	static void ProcessDeletionQueue() {
		s_RendererAPI->ProcessDeletionQueue();
	}

	static void EnablePolygonOffset() {
		s_RendererAPI->EnablePolygonOffset();
	}

	static void PolygonOffset(float factor, float units) {
		s_RendererAPI->PolygonOffset(factor, units);
	}

	static void DisablePolygonOffset() {
		s_RendererAPI->DisablePolygonOffset();
	}
private:
	 static Scope<RendererAPI> s_RendererAPI;
};