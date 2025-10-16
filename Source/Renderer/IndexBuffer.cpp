#include "IndexBuffer.h"

#include "Core/Log.h"

#include "Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"

Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, const uint32_t count) {
	Log::Trace("IndexBuffer::Create - Creating Index Buffer");

	switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indices, count);
	}

	Log::Error("IndexBuffer::Create - Unknown Renderer API");

	return nullptr;
}