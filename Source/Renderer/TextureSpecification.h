#pragma once

#include <cstdint>
#include <filesystem>

// Texture Format Specification for Framebuffers and Textures
enum class TextureFormat {
	None = 0,
	// Color Format
	R8,
	RGB8,
	RGBA8,
	RGBA16F,
	// Depth/Stencil Format
	Depth24Stencil8
};

// Magnification and Minification Filter options
enum class TextureFilter {
	None = 0,
	Nearest, // Without interpolation (pixelated)
	Linear,   // With interpolation (smooth)
	NearestMipmapNearest, // Without interpolation, chooses the closest mipmap
	LinearMipmapNearest,  // With interpolation, chooses the closest mipmap
	NearestMipmapLinear,  // Without interpolation, smooth between two mipmaps
	LinearMipmapLinear	  // With interpolation, smooth between two mipmaps (Trilinear filtering)
};

// Wrapping options for Texture Coordinates
enum class TextureWrap {
	None = 0,
	Repeat,
	ClampToEdge,
	ClampToBorder
};

struct TextureSpecification {
	uint32_t Width = 1;
	uint32_t Height = 1;

	TextureFormat Format = TextureFormat::RGBA8;

	TextureFilter MinFilter = TextureFilter::LinearMipmapLinear;
	TextureFilter MagFilter = TextureFilter::Linear;

	TextureWrap WrapS = TextureWrap::Repeat;
	TextureWrap WrapT = TextureWrap::Repeat;

	bool GenerateMips = true;
};