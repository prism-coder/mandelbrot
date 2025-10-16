#pragma once

#include "Core/Core.h"

class IndexBuffer {
public:
	virtual ~IndexBuffer() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual uint32_t GetCount() const = 0;

	static Ref<IndexBuffer> Create(const uint32_t* indices, const uint32_t count);
};