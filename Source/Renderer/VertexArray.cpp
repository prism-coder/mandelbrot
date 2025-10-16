#include "VertexArray.h"

#include "Core/Log.h"

#include "Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

Ref<VertexArray> VertexArray::Create() {
	Log::Trace("VertexArray::Create - Creating Vertex Array");

	switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexArray>();
	}

	Log::Error("VertexArray::Create - Unknown Renderer API");

	return nullptr;
}