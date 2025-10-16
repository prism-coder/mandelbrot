#pragma once

#include "Renderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI {
public:
	virtual void Init() override;
	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

	virtual void SetClearColor(const glm::vec4& color) override;
	virtual void Clear() override;
	virtual void ClearDepth() override;

	virtual void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
	virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) override;

	virtual void DispatchCompute(uint32_t groupX, uint32_t groupY, uint32_t groupZ) override;
	virtual void BlitFramebufferToSwapchain(uint32_t fbo, uint32_t width, uint32_t height) override;

	virtual void EnableDepthTest(const bool& enable) override;
	virtual void EnableDepthMask(const bool& enable) override;
	virtual void EnableCullFace(const bool& enable) override;

	virtual void SetDepthFunction(const DepthFunction& depthFunction) override;

	virtual void QueueTextureForDeletion(uint32_t handle) override;
	virtual void ProcessDeletionQueue() override;

	virtual void EnablePolygonOffset() override;
	virtual void PolygonOffset(float factor, float units) override;
	virtual void DisablePolygonOffset() override;
private:
	std::vector<uint32_t> m_TextureDeletionQueue;
};