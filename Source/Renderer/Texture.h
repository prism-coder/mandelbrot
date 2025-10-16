#pragma once

#include "Core/Core.h"

#include "Renderer/TextureSpecification.h"

#include <filesystem>

class Texture {
public:
	virtual ~Texture() = default;

	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;
	virtual uint32_t GetHandle() const = 0;
	virtual std::filesystem::path GetFilepath() const = 0;

	virtual void Bind(uint32_t slot = 0) const = 0;
	virtual void BindToImageUnit(uint32_t unit, bool read, bool write) = 0;
};

class Texture2D : public Texture {
public:
	static Ref<Texture2D> Create(const TextureSpecification& specification);
	static Ref<Texture2D> Create(const std::filesystem::path& filepath, const TextureSpecification& specification = {});
};