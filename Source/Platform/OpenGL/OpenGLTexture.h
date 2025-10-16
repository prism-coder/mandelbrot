#pragma once

#include "Renderer/Texture.h"

#include "Renderer/TextureSpecification.h"

#include <glad/glad.h>

class OpenGLTexture2D : public Texture2D {
public:
	OpenGLTexture2D(const TextureSpecification& specification = {});
	OpenGLTexture2D(const std::filesystem::path& filepath, const TextureSpecification& specification = {});
	virtual ~OpenGLTexture2D();

	virtual uint32_t GetWidth() const override { return m_Width; }
	virtual uint32_t GetHeight() const override { return m_Height; }
	virtual uint32_t GetHandle() const override { return m_Handle; }
	virtual std::filesystem::path GetFilepath() const override { return m_Filepath; }

	virtual void Bind(uint32_t slot = 0) const override;
	virtual void BindToImageUnit(uint32_t unit, bool read, bool write) override;
private:
	GLuint m_Handle = 0;
	uint32_t m_Width = 0, m_Height = 0;

	std::filesystem::path m_Filepath;

	TextureSpecification m_Specification;
};