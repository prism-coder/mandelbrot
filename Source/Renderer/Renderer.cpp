#include "Renderer.h"

#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Settings/SettingsManager.h"

#include "stb_image_write.h"

void Renderer::Init() {
	Log::Trace("Renderer::Init - Initializing the Renderer");
	RenderCommand::Init();

	InitFramebuffer();
	InitVertexArray();
	InitShader();

	RenderCommand::EnableDepthTest(true);
}

void Renderer::Shutdown() {
	Log::Trace("Renderer::Shutdown - Shutting down the Renderer");

	s_Framebuffer.reset();
}

void Renderer::Begin() {
	RenderCommand::ProcessDeletionQueue();

	s_Framebuffer->Bind();

	RenderCommand::Clear();
}

void Renderer::End() {
	// Unbind our framebuffer
	s_Framebuffer->Unbind();
}

void Renderer::Submit(const Mandelbrot& mandelbrot) {
	if (!m_Shader || ! m_QuadVA) {
		return;
	}

	const auto& width = s_Framebuffer->GetWidth();
	const auto& height = s_Framebuffer->GetHeight();

	m_Shader->Bind();

	// View and Calculation
	m_Shader->SetUniform("u_Resolution", glm::vec2((float)width, (float)height));
	m_Shader->SetUniform("u_Zoom", mandelbrot.Zoom);
	m_Shader->SetUniform("u_Position", mandelbrot.Position);
	m_Shader->SetUniform("u_Rotation", glm::radians(mandelbrot.Rotation));
	m_Shader->SetUniform("u_MaxIterations", mandelbrot.MaxIterations);
	m_Shader->SetUniform("u_Bailout", mandelbrot.Bailout);
	m_Shader->SetUniform("u_Algorithm", static_cast<int>(mandelbrot.Algorithm));
	m_Shader->SetUniform("u_Power", mandelbrot.Power);

	// Julia
	m_Shader->SetUniform("u_JuliaMode", mandelbrot.JuliaMode);
	m_Shader->SetUniform("u_JuliaC", mandelbrot.JuliaC);

	// Coloration
	m_Shader->SetUniform("u_ExteriorColoring", static_cast<int>(mandelbrot.ExteriorColoring));
	m_Shader->SetUniform("u_InteriorColoring", static_cast<int>(mandelbrot.InteriorColoring));
	m_Shader->SetUniform("u_InteriorColor", mandelbrot.InteriorColor);
	m_Shader->SetUniform("u_ColorFrequency", mandelbrot.ColorFrequency);
	m_Shader->SetUniform("u_ColorOffset", mandelbrot.ColorOffset);
	m_Shader->SetUniform("u_OrbitColoring", mandelbrot.OrbitColoring);
	m_Shader->SetUniform("u_DistanceScale", mandelbrot.DistanceScale);

	// Pallette
	m_Shader->SetUniform("u_ColorCount", mandelbrot.ColorPalette.ColorCount);
	for (size_t i = 0; i < mandelbrot.ColorPalette.ColorCount; ++i) {
		std::string colorsName = "u_Colors[" + std::to_string(i) + "]";
		std::string positionsName = "u_ColorPositions[" + std::to_string(i) + "]";

		m_Shader->SetUniform(colorsName, mandelbrot.ColorPalette.ColorData[i]);
		m_Shader->SetUniform(positionsName, mandelbrot.ColorPalette.ColorPositions[i]);
	}

	// Orbit Trap
	m_Shader->SetUniform("u_TrapType", static_cast<int>(mandelbrot.Trap.Type));
	m_Shader->SetUniform("u_TrapP1", mandelbrot.Trap.P1);
	m_Shader->SetUniform("u_TrapP2", mandelbrot.Trap.P2);
	m_Shader->SetUniform("u_TrapColor", mandelbrot.Trap.Color);
	m_Shader->SetUniform("u_TrapBlend", mandelbrot.Trap.Blend);

	RenderCommand::DrawIndexed(m_QuadVA);
}

void Renderer::ExportFrame(const std::filesystem::path& filepath) {
	if (!s_Framebuffer) {
		Log::Error("Renderer::ExportFrame - Cannot export, framebuffer is null.");
		return;
	}

	uint32_t width = s_Framebuffer->GetWidth();
	uint32_t height = s_Framebuffer->GetHeight();

	if (width == 0 || height == 0) {
		Log::Warning("Renderer::ExportFrame - Cannot export a frame with zero size.");
		return;
	}

	// Create a buffer in the CPU memory to store the pixels.
	// The size is width * height * 4 (because we use RGBA, 4 channels of 1 byte).
	std::vector<uint8_t> pixels(width * height * 4);

	// Tell OpenGL that we want to read from our framebuffer.
	s_Framebuffer->Bind();
	// Read the pixels from the framebuffer to our CPU buffer.
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	// Stop using the framebuffer.
	s_Framebuffer->Unbind();

	// Tell STB to flip the image vertically when saving it.
	// This is necessary because the OpenGL coordinate system(0, 0 down) is
	// inverted compared to most image formats(0, 0 up).
	stbi_flip_vertically_on_write(1);

	// Use STB to write the pixels to a PNG file.
	stbi_write_png(filepath.string().c_str(), width, height, 4, pixels.data(), width * 4);
}

void Renderer::InitFramebuffer() {
	Log::Trace("Renderer::InitFramebuffer - Initializing the Framebuffer");
	Log::Trace("Renderer::InitFramebuffer - Setting up the Framebuffer Texture Specification");

	TextureSpecification textureSpec;
	textureSpec.Width = Application::Get().GetWidth();
	textureSpec.Height = Application::Get().GetHeight();
	textureSpec.Format = TextureFormat::RGBA16F;
	textureSpec.MinFilter = TextureFilter::Linear;
	textureSpec.MagFilter = TextureFilter::Linear;
	textureSpec.WrapS = TextureWrap::ClampToEdge;
	textureSpec.WrapT = TextureWrap::ClampToEdge;
	textureSpec.GenerateMips = false;

	Log::Trace("Renderer::InitFramebuffer - Setting up the Framebuffer");
	FramebufferSpecification fbSpec;
	fbSpec.Width = textureSpec.Width;
	fbSpec.Height = textureSpec.Height;
	fbSpec.ColorAttachmentSpecification = textureSpec;
	fbSpec.HasDepthAttachment = true;
	fbSpec.DepthAttachmentSpecification.Format = TextureFormat::Depth24Stencil8;

	Log::Trace("Renderer::InitFramebuffer - Creating the Framebuffer");
	s_Framebuffer = Framebuffer::Create(fbSpec);
}

void Renderer::InitVertexArray() {
	Log::Trace("Renderer::InitFramebuffer - Initializing Fullscreen Vertex Array");

	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f
	};

	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

	Ref<VertexBuffer> quadVB = VertexBuffer::Create(vertices, sizeof(vertices));
	BufferLayout layout = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" }
	};
	quadVB->SetLayout(layout);

	m_QuadVA = VertexArray::Create();
	m_QuadVA->AddVertexBuffer(quadVB);

	Ref<IndexBuffer> quadIB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

	m_QuadVA->SetIndexBuffer(quadIB);
}

void Renderer::InitShader() {
	Log::Trace("Renderer::InitFramebuffer - Initializing Shader");

	m_Shader = Shader::CreateGraphics(
		"Internal/Shaders/Mandelbrot/Mandelbrot.vert",
		"Internal/Shaders/Mandelbrot/Mandelbrot.frag"
	);
}
