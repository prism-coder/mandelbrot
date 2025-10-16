#include "Framebuffer.h"

#include "Core/Log.h"

#include "Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
	Log::Trace("Framebuffer::Create - Creating the Framebuffer");

	switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFramebuffer>(spec);
	}

	Log::Error("Framebuffer::Create - Unknown Renderer API");

	return nullptr;
}
