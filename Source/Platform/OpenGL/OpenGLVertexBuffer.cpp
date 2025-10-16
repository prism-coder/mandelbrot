#include "OpenGLVertexBuffer.h"

#include "Core/Log.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, const uint32_t size) {
	Log::Trace("OpenGLVertexBuffer::OpenGLVertexBuffer - Creating OpenGL Vertex Buffer");

	glCreateBuffers(1, &m_Handle);
	glNamedBufferData(m_Handle, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
	glDeleteBuffers(1, &m_Handle);
}

void OpenGLVertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
}

void OpenGLVertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}