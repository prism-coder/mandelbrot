#include "OpenGLIndexBuffer.h"

#include "Core/Log.h"

OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, const uint32_t count) : m_Count(count) {
	Log::Trace("OpenGLIndexBuffer::OpenGLIndexBuffer - Creating OpenGL Index Buffer");

	glCreateBuffers(1, &m_Handle);
	glNamedBufferData(m_Handle, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
	glDeleteBuffers(1, &m_Handle);
}

void OpenGLIndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
}

void OpenGLIndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}