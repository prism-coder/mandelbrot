#include "OpenGLFramebuffer.h"

#include "Core/Log.h"

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
	: m_Specification(spec)
{
	Log::Trace("OpenGLFramebuffer - Creating the OpenGL Framebuffer");
	Invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
	Log::Trace("OpenGLFramebuffer - Deleting the OpenGL Framebuffer");
	glDeleteFramebuffers(1, &m_Handle);
}

void OpenGLFramebuffer::Invalidate() {
	Log::Trace("OpenGLFramebuffer::Invalidate - Invalidating the OpenGL Framebuffer");

	// If the Framebuffer exists, we delete it and it's attachments
	if (m_Handle) {
		glDeleteFramebuffers(1, &m_Handle);

		m_ColorAttachment = nullptr;
		m_DepthAttachment = nullptr;
	}

	// Create the framebuffer
	glCreateFramebuffers(1, &m_Handle);

	// Create the Color attachment only if it has a texture format
	if (m_Specification.ColorAttachmentSpecification.Format != TextureFormat::None) {
		m_Specification.ColorAttachmentSpecification.Width = m_Specification.Width;
		m_Specification.ColorAttachmentSpecification.Height = m_Specification.Height;
		m_ColorAttachment = Texture2D::Create(m_Specification.ColorAttachmentSpecification);
		glNamedFramebufferTexture(m_Handle, GL_COLOR_ATTACHMENT0, m_ColorAttachment->GetHandle(), 0);
	}

	// Create the Depth attachment only if there is one
	if (m_Specification.HasDepthAttachment) {
		m_Specification.DepthAttachmentSpecification.Width = m_Specification.Width;
		m_Specification.DepthAttachmentSpecification.Height = m_Specification.Height;
		m_DepthAttachment = Texture2D::Create(m_Specification.DepthAttachmentSpecification);
		glNamedFramebufferTexture(m_Handle, GL_DEPTH_STENCIL_ATTACHMENT, m_DepthAttachment->GetHandle(), 0);
	}

	if (m_Specification.ColorAttachmentSpecification.Format == TextureFormat::None) {
		glNamedFramebufferDrawBuffer(m_Handle, GL_NONE);
		glNamedFramebufferReadBuffer(m_Handle, GL_NONE);
	}

	// Check if the Framebuffer has been created successfully
	if (glCheckNamedFramebufferStatus(m_Handle, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log::Error("OpenGLFramebuffer::Invalidate - Framebuffer is not complete");
	}
}

void OpenGLFramebuffer::Bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
	glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void OpenGLFramebuffer::Unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
	if (width == 0 || height == 0) return;

	// Update the specification
	m_Specification.Width = width;
	m_Specification.Height = height;

	// We need to recreate the framebuffer
	Invalidate();
}