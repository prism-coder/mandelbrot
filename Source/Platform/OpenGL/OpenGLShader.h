#pragma once

#include "Renderer/Shader.h"
#include <glad/glad.h>

class OpenGLShader : public Shader {
public:
	OpenGLShader(const std::filesystem::path& computePath);
	OpenGLShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
	virtual ~OpenGLShader();

	virtual void Bind() const override;
	virtual void Unbind() const override;
	virtual void Reload() override;

	virtual const std::filesystem::path& GetFilepath() const override { return m_ShaderAssetPath; }
private:
	virtual void UploadUniformInt(const std::string& name, int value) override;
	virtual void UploadUniformFloat(const std::string& name, float value) override;
	virtual void UploadUniformBool(const std::string& name, bool value) override;
	virtual void UploadUniformVec2(const std::string& name, const glm::vec2& value) override;
	virtual void UploadUniformVec3(const std::string& name, const glm::vec3& value) override;
	virtual void UploadUniformVec4(const std::string& name, const glm::vec4& value) override;
	virtual void UploadUniformMat4(const std::string& name, const glm::mat4& value) override;

	GLuint CompileShader(GLenum type, const std::string& source);
	std::string ReadTextFile(const std::filesystem::path& path);
private:
	GLuint m_Handle = 0;
	std::filesystem::path m_ComputePath;
	std::filesystem::path m_VertexPath, m_FragmentPath;
	std::filesystem::path m_ShaderAssetPath;
	bool m_IsCompute = false;
};