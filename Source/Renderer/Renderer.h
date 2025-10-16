#pragma once

#include "Renderer/RenderCommand.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include "Layers/Mandelbrot/Mandelbrot.h"

#include <filesystem>

class Renderer {
public:
	static void Init();
	static void Shutdown();

	static void Begin();
	static void End();

	static void Submit(const Mandelbrot& mandelbrot);
	static void ExportFrame(const std::filesystem::path& filepath);

	static Ref<Framebuffer> GetFramebuffer() { return s_Framebuffer; }
private:
	static void InitFramebuffer();
	static void InitVertexArray();
	static void InitShader();
private:
	inline static Ref<Framebuffer> s_Framebuffer = nullptr;
	inline static Ref<VertexArray> m_QuadVA = nullptr;
	inline static Ref<Shader> m_Shader = nullptr;
};