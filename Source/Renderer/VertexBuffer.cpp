#include "VertexBuffer.h"

#include "Core/Log.h"

#include "Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

Ref<VertexBuffer> VertexBuffer::Create(const float* vertices, const uint32_t size) {
	Log::Trace("VertexBuffer::Create - Creating Vertex Buffer");

	switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(vertices, size);
	}

	Log::Error("VertexBuffer::Create - Unknown Renderer API");

	return nullptr;
}