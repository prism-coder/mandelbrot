#pragma once

#include "Renderer/VertexArray.h"

#include <glm/glm.hpp>

enum class DepthFunction {
	Less,   // Draw if the new pixel is closer (default)
	LEqual	// Draw if closer OR at the same depth (useful for skybox/planes)
};

class RendererAPI {
public:
	enum class API {
		None = 0, OpenGL = 1
	};
public:
	virtual ~RendererAPI() = default;

	virtual void Init() = 0;
	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

	virtual void SetClearColor(const glm::vec4& color) = 0;
	virtual void Clear() = 0;
	virtual void ClearDepth() = 0;

	virtual void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
	virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

	virtual void DispatchCompute(uint32_t groupX, uint32_t groupY, uint32_t groupZ) = 0;
	virtual void BlitFramebufferToSwapchain(uint32_t fbo, uint32_t width, uint32_t height) = 0;

	virtual void EnableDepthTest(const bool& enable) = 0;
	virtual void EnableDepthMask(const bool& enable) = 0;
	virtual void EnableCullFace(const bool& enable) = 0;

	virtual void SetDepthFunction(const DepthFunction& depthFunction) = 0;

	virtual void QueueTextureForDeletion(uint32_t handle) = 0;
	virtual void ProcessDeletionQueue() = 0;

	virtual void EnablePolygonOffset() = 0;
	virtual void PolygonOffset(float factor, float units) = 0;
	virtual void DisablePolygonOffset() = 0;

	static API GetAPI() { return s_API; }
private:
	static API s_API;
};