#pragma once

#include "Core/Core.h"

#include "Renderer/BufferLayout.h"

class VertexBuffer {
public:
	virtual ~VertexBuffer() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void SetLayout(const BufferLayout& layout) = 0;
	virtual const BufferLayout& GetLayout() const = 0;

	static Ref<VertexBuffer> Create(const float* vertices, const uint32_t size);
};