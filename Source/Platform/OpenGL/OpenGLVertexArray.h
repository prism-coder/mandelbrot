#pragma once

#include "Renderer/VertexArray.h"

#include <glad/glad.h>
#include <vector>

class OpenGLVertexArray : public VertexArray {
public:
	OpenGLVertexArray();
	virtual ~OpenGLVertexArray();
	virtual void Bind() const override;
	virtual void Unbind() const override;
	virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
	virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
	virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; };

	GLuint GetHandle() const { return m_Handle; }
private:
	GLuint m_Handle = 0;
	Ref<IndexBuffer> m_IndexBuffer;

	std::vector<Ref<VertexBuffer>> m_VertexBuffers;
};