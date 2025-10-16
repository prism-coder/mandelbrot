#include "OpenGLRendererAPI.h"

#include "Core/Log.h"

#include <glad/glad.h>
#include <iostream>

static std::string TypeToString(GLenum type) {
	switch (type) {
		case GL_DEBUG_TYPE_ERROR:				return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED BEHAVIOUR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "UNDEFINED BEHAVIOUR";
		case GL_DEBUG_TYPE_PORTABILITY:			return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE:			return "PERFORMANCE";
		case GL_DEBUG_TYPE_OTHER:				return "OTHER";
		default:								return "UNKNOWN";
	}
}

static std::string SeverityToString(GLenum severity) {
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:			return "HIGH";
		case GL_DEBUG_SEVERITY_MEDIUM:			return "MEDIUM";
		case GL_DEBUG_SEVERITY_LOW:				return "LOW";
		case GL_DEBUG_SEVERITY_NOTIFICATION:	return "NOTIFICATION";
		default:								return "UNKNOWN";
	}
}

static GLenum DepthFunctionToGLEnum(const DepthFunction& depthFunction) {
	switch (depthFunction) {
		case DepthFunction::Less:	return GL_LESS;
		case DepthFunction::LEqual: return GL_LEQUAL;
	}

	Log::Error("OpenGLRendererAPI::DepthFunctionToGLEnum - Unknown DepthFunction");

	return GL_LESS;
}

void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
		return;
	}

	Log::Error("OpenGLRendererAPI - Type: " + TypeToString(type) + ", Severity: " + SeverityToString(severity) + ", Message: " + message);
}

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

void OpenGLRendererAPI::Init() {
	Log::Trace("OpenGLRendererAPI::Init - Initializing OpenGL RendererAPI");

	if (!gladLoadGL()) {
		Log::Error("OpenGLRendererAPI::Init - Failed to initialize GLAD");
		return;
	}

	Log::Trace("OpenGLRendererAPI::Init - Enabling OpenGL debug messages");

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GLDebugMessageCallback, 0);
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::ClearDepth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) {
	if (vertexArray) {
		vertexArray->Bind();
	}

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
	vertexArray->Bind();
	uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::DispatchCompute(uint32_t groupX, uint32_t groupY, uint32_t groupZ) {
	glDispatchCompute(groupX, groupY, groupZ);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void OpenGLRendererAPI::BlitFramebufferToSwapchain(uint32_t fbo, uint32_t width, uint32_t height) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void OpenGLRendererAPI::EnableDepthTest(const bool& enable) {
	if (enable) {
		glEnable(GL_DEPTH_TEST);
	} else {
		glDisable(GL_DEPTH_TEST);
	}
}

void OpenGLRendererAPI::EnableDepthMask(const bool& enable) {
	if (enable) {
		glDepthMask(GL_TRUE);
	} else {
		glDepthMask(GL_FALSE);
	}
}

void OpenGLRendererAPI::EnableCullFace(const bool& enable) {
	if (enable) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
}

void OpenGLRendererAPI::SetDepthFunction(const DepthFunction& depthFunction) {
	glDepthFunc(DepthFunctionToGLEnum(depthFunction));
}

void OpenGLRendererAPI::QueueTextureForDeletion(uint32_t handle) {
	m_TextureDeletionQueue.push_back(handle);
}

void OpenGLRendererAPI::ProcessDeletionQueue() {
	if (!m_TextureDeletionQueue.empty()) {
		glDeleteTextures((GLsizei)m_TextureDeletionQueue.size(), m_TextureDeletionQueue.data());
		m_TextureDeletionQueue.clear();
	}
}

void OpenGLRendererAPI::EnablePolygonOffset() {
	glEnable(GL_POLYGON_OFFSET_FILL);
}

void OpenGLRendererAPI::PolygonOffset(float factor, float units) {
	glPolygonOffset(factor, units);
}

void OpenGLRendererAPI::DisablePolygonOffset() {
	glDisable(GL_POLYGON_OFFSET_FILL);
}
