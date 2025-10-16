#include "OpenGLTexture.h"

#include "Core/Log.h"

#include "Renderer/RenderCommand.h"

#include <stb_image.h>
#include <iostream>

static GLenum TextureFormatToGLInternalFormat(TextureFormat format) {
	switch (format) {
		case TextureFormat::R8:					return GL_R8;
		case TextureFormat::RGB8:				return GL_RGB8;
		case TextureFormat::RGBA8:				return GL_RGBA8;
		case TextureFormat::RGBA16F:			return GL_RGBA16F;
		case TextureFormat::Depth24Stencil8:	return GL_DEPTH24_STENCIL8;
		default:								return 0;
	}
}

static GLenum TextureFormatToGLDataFormat(TextureFormat format) {
	switch (format) {
		case TextureFormat::R8:					return GL_RED;
		case TextureFormat::RGB8:				return GL_RGB;
		case TextureFormat::RGBA8:				return GL_RGBA;
		case TextureFormat::RGBA16F:			return GL_RGBA;
		case TextureFormat::Depth24Stencil8:	return GL_DEPTH_STENCIL;
		default:								return 0;
	}
}

static GLenum TextureFormatToGLDataType(TextureFormat format) {
	switch (format) {
		case TextureFormat::R8:
		case TextureFormat::RGB8:
		case TextureFormat::RGBA8:				return GL_UNSIGNED_BYTE;
		case TextureFormat::RGBA16F:			return GL_FLOAT;
		case TextureFormat::Depth24Stencil8:	return GL_UNSIGNED_INT_24_8;
		default:								return 0;
	}
}

static GLenum TextureFilterToGLFilter(TextureFilter filter) {
	switch (filter) {
		case TextureFilter::Nearest:				return GL_NEAREST;
		case TextureFilter::Linear:					return GL_LINEAR;
		case TextureFilter::NearestMipmapNearest:	return GL_NEAREST_MIPMAP_NEAREST;
		case TextureFilter::LinearMipmapNearest:	return GL_LINEAR_MIPMAP_NEAREST;
		case TextureFilter::NearestMipmapLinear:	return GL_NEAREST_MIPMAP_LINEAR;
		case TextureFilter::LinearMipmapLinear:		return GL_LINEAR_MIPMAP_LINEAR;
		default:									return 0;
	}
}

static GLenum TextureWrapToGLWrap(TextureWrap wrap) {
	switch (wrap) {
		case TextureWrap::Repeat:			return GL_REPEAT;
		case TextureWrap::ClampToEdge:		return GL_CLAMP_TO_EDGE;
		case TextureWrap::ClampToBorder:	return GL_CLAMP_TO_BORDER;
		default:							return 0;
	}
}

Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification) {
	Log::Trace("Texture2D::Create - Creating Texture from specification");

	return CreateRef<OpenGLTexture2D>(specification);
}

Ref<Texture2D> Texture2D::Create(const std::filesystem::path& filepath, const TextureSpecification& specification) {
	Log::Trace("Texture2D::Create - Creating Texture from path");
	
	return CreateRef<OpenGLTexture2D>(filepath, specification);
}

// Constructor for empty texture
OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
	: m_Width(specification.Width), m_Height(specification.Height), m_Specification(specification)
{
	Log::Trace("OpenGLTexture2D::OpenGLTexture2D - Creating OpenGL Texture from specification");
	
	GLenum internalFormat = TextureFormatToGLInternalFormat(specification.Format);
	GLenum dataFormat = TextureFormatToGLDataFormat(specification.Format);
	GLenum dataType = TextureFormatToGLDataType(specification.Format);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
	glTextureStorage2D(m_Handle, 1, internalFormat, m_Width, m_Height);

	glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, TextureFilterToGLFilter(specification.MinFilter));
	glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, TextureFilterToGLFilter(specification.MagFilter));
	glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, TextureWrapToGLWrap(specification.WrapS));
	glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, TextureWrapToGLWrap(specification.WrapT));

	if (specification.Format == TextureFormat::Depth24Stencil8) {
		glTextureParameteri(m_Handle, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTextureParameteri(m_Handle, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTextureParameterfv(m_Handle, GL_TEXTURE_BORDER_COLOR, borderColor);
	}
}

// Constructor for loading from file
OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& filepath, const TextureSpecification& specification)
	: m_Filepath(filepath), m_Specification(specification)
{
	Log::Trace("OpenGLTexture2D::OpenGLTexture2D - Creating OpenGL Texture from path");

	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);

	void* data = nullptr;
	bool isHDR = filepath.extension() == ".hdr";

	if (isHDR) {
		data = stbi_loadf(filepath.string().c_str(), &width, &height, &channels, 0);
	} else {
		data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);
	}

	if (!data) {
		Log::Error("OpenGLTexture2D::OpenGLTexture2D - Failed to load texture: " + filepath.string());
		return;
	}

	m_Width = width;
	m_Height = height;

	GLenum internalFormat = 0, dataFormat = 0, dataType = 0;
	if (isHDR) {
		internalFormat = GL_RGB16F;
		dataFormat = GL_RGB;
		dataType = GL_FLOAT;
	} else if (channels == 4) {
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
		dataType = GL_UNSIGNED_BYTE;
	} else if (channels == 3) {
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
		dataType = GL_UNSIGNED_BYTE;
	} else if (channels == 1) {
		internalFormat = GL_R8;
		dataFormat = GL_RED;
		dataType = GL_UNSIGNED_BYTE;
	}
	
	if (internalFormat == 0 || dataFormat == 0 || dataType == 0) {
		Log::Error("OpenGLTexture2D::OpenGLTexture2D - Unsupported image format (channels): " + std::to_string(channels));
		stbi_image_free(data);
		return;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
	glTextureStorage2D(m_Handle, 1, internalFormat, m_Width, m_Height);

	GLenum minFilterGL = TextureFilterToGLFilter(specification.MinFilter);
	GLenum magFilterGL = TextureFilterToGLFilter(specification.MagFilter);

	if (
		magFilterGL == GL_LINEAR_MIPMAP_LINEAR ||
		magFilterGL == GL_LINEAR_MIPMAP_NEAREST ||
		magFilterGL == GL_NEAREST_MIPMAP_LINEAR ||
		magFilterGL == GL_NEAREST_MIPMAP_NEAREST
	) {
		magFilterGL = GL_LINEAR;
	}

	glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, minFilterGL);
	glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, magFilterGL);
	glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, TextureWrapToGLWrap(specification.WrapS));
	glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, TextureWrapToGLWrap(specification.WrapT));

	glTextureSubImage2D(m_Handle, 0, 0, 0, m_Width, m_Height, dataFormat, dataType, data);

	if (specification.GenerateMips) {
		if (
			minFilterGL == GL_LINEAR_MIPMAP_LINEAR ||
			minFilterGL == GL_LINEAR_MIPMAP_NEAREST ||
			minFilterGL == GL_NEAREST_MIPMAP_LINEAR ||
			minFilterGL == GL_NEAREST_MIPMAP_NEAREST
		) {
			Log::Trace("OpenGLTexture2D::OpenGLTexture2D - Generating mips for Texture");
			glGenerateTextureMipmap(m_Handle);
		}
	}

	stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() {
	Log::Trace("OpenGLTexture2D::~OpenGLTexture2D - Deleting OpenGL Texture");

	RenderCommand::SubmitTextureForDeletion(m_Handle);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
	glBindTextureUnit(slot, m_Handle);
}

void OpenGLTexture2D::BindToImageUnit(uint32_t unit, bool read, bool write) {
	GLenum access = GL_READ_ONLY;
	if (read && write) access = GL_READ_WRITE;
	else if (write) access = GL_WRITE_ONLY;

	glBindImageTexture(unit, m_Handle, 0, GL_FALSE, 0, access, GL_RGBA32F);
}