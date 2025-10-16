#include "OpenGLVertexArray.h"

#include "OpenGLVertexBuffer.h"

#include "Core/Log.h"

#include <cassert>

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Bool:		return GL_BOOL;
	}

	Log::Trace("ShaderDataTypeToOpenGLBaseType - Unknown Shader Data Type");

	return 0;
}

OpenGLVertexArray::OpenGLVertexArray() {
	Log::Trace("OpenGLVertexArray::OpenGLVertexArray - Creating OpenGL Vertex Array");

	glCreateVertexArrays(1, &m_Handle);

	if (m_Handle == 0) {
		Log::Error("OpenGLVertexArray::OpenGLVertexArray - Failed while creating the Vertex Array Object (VAO)");
		assert(false);
	}
}

OpenGLVertexArray::~OpenGLVertexArray() {
	glDeleteVertexArrays(1, &m_Handle);
}

void OpenGLVertexArray::Bind() const {
	glBindVertexArray(m_Handle);
}

void OpenGLVertexArray::Unbind() const {
	glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
	if (vertexBuffer->GetLayout().GetElements().empty()) {
		Log::Error("OpenGLVertexArray::AddVertexBuffer - Vertex Buffer has no layout");
		assert(false);
	}

	glBindVertexArray(m_Handle);
	vertexBuffer->Bind();

	const auto& layout = vertexBuffer->GetLayout();
	uint32_t attribIndex = 0;
	uint32_t bindingIndex = m_VertexBuffers.size();

	for (const auto& element : layout) {
		glEnableVertexArrayAttrib(m_Handle, attribIndex);
		glVertexArrayAttribFormat(
			m_Handle,
			attribIndex,
			element.GetComponentCount(),
			ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			element.Offset
		);
		glVertexArrayAttribBinding(m_Handle, attribIndex, bindingIndex);
		attribIndex++;
	}

	auto vbHandle = std::static_pointer_cast<OpenGLVertexBuffer>(vertexBuffer)->GetHandle();
	glVertexArrayVertexBuffer(m_Handle, bindingIndex, vbHandle, 0, layout.GetStride());

	m_VertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
	glBindVertexArray(m_Handle);
	indexBuffer->Bind();
	m_IndexBuffer = indexBuffer;
}