#pragma once

#include "Renderer/VertexBuffer.h"

#include <glad/glad.h>

class OpenGLVertexBuffer : public VertexBuffer {
public:
	OpenGLVertexBuffer(const float* vertices, const uint32_t size);
	virtual ~OpenGLVertexBuffer();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	virtual const BufferLayout& GetLayout() const override { return m_Layout; }

	GLuint GetHandle() const { return m_Handle; }
private:
	GLuint m_Handle = 0;
	BufferLayout m_Layout;
};