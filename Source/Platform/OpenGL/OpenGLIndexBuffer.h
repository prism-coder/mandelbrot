#pragma once

#include "Renderer/IndexBuffer.h"

#include <glad/glad.h>

class OpenGLIndexBuffer : public IndexBuffer {
public:
	OpenGLIndexBuffer(const uint32_t* indices, const uint32_t count);
	virtual ~OpenGLIndexBuffer();
	virtual void Bind() const override;
	virtual void Unbind() const override;
	virtual uint32_t GetCount() const override { return m_Count; }

	GLuint GetHandle() const { return m_Handle; }
private:
	GLuint m_Handle = 0;
	uint32_t m_Count;
};