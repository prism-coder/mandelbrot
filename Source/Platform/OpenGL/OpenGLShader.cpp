#include "OpenGLShader.h"

#include "Core/Log.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "yaml-cpp/yaml.h"

std::string ShaderTypeToString(GLenum type) {
	switch (type) {
		case GL_VERTEX_SHADER:		return "Vertex";
		case GL_FRAGMENT_SHADER:	return "Fragment";
		case GL_COMPUTE_SHADER:		return "Compute";
		default:					return "Unknown";
	}
}

Ref<Shader> Shader::Create(const std::filesystem::path& shaderAssetPath) {
	Log::Trace("Shader::Create - Creating Shader from Asset File: " + shaderAssetPath.string());

	YAML::Node data;
	try {
		data = YAML::LoadFile(shaderAssetPath.string());
	} catch (YAML::Exception e) {
		Log::Error("Shader::Create - Failed to load Ahader Asset file: " + e.msg);
		return nullptr;
	}

	if (auto shaderNode = data["Shader"]) {
		std::string vertexPath = shaderNode["VertexPath"].as<std::string>();
		std::string fragmentPath = shaderNode["FragmentPath"].as<std::string>();

		if (vertexPath.empty() || fragmentPath.empty()) {
			Log::Error("Shader::Create - Shader Asset file is missing vertex or fragment path.");
			return nullptr;
		}

		return CreateGraphics(vertexPath, fragmentPath);
	}

	Log::Error("Shader::Create - Malformed Shader Asset file: " + shaderAssetPath.string());

	return nullptr;
}

Ref<Shader> Shader::CreateCompute(const std::filesystem::path& path) {
	Log::Trace("Shader::CreateCompute - Creating Compute Shader");

	return CreateRef<OpenGLShader>(path);
}

Ref<Shader> Shader::CreateGraphics(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
	Log::Trace("Shader::CreateGraphics - Creating Graphics Shader");

	return CreateRef<OpenGLShader>(vertexPath, fragmentPath);
}

// Constructor for Compute Shaders
OpenGLShader::OpenGLShader(const std::filesystem::path& computePath)
	: m_ComputePath(computePath), m_IsCompute(true) {
	Reload();
}

// Constructor for Graphics Shaders
OpenGLShader::OpenGLShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	: m_VertexPath(vertexPath), m_FragmentPath(fragmentPath), m_IsCompute(false) {
	Reload();
}

OpenGLShader::~OpenGLShader() {
	glDeleteProgram(m_Handle);
}

void OpenGLShader::Reload() {
	Log::Trace("OpenGLShader::Reload - Reloading Shader");

	GLuint newProgramHandle = 0;
	if (m_IsCompute) {
		Log::Trace("OpenGLShader::Reload - Reloading Compute Shader");

		std::string source = ReadTextFile(m_ComputePath);
		GLuint computeShader = CompileShader(GL_COMPUTE_SHADER, source);
		if (computeShader == 0) return;

		newProgramHandle = glCreateProgram();
		glAttachShader(newProgramHandle, computeShader);
		glLinkProgram(newProgramHandle);
		glDeleteShader(computeShader);
	} else {
		Log::Trace("OpenGLShader::Reload - Reloading Graphics Shader");

		std::string vertexSource = ReadTextFile(m_VertexPath);
		std::string fragmentSource = ReadTextFile(m_FragmentPath);
		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
		if (vertexShader == 0 || fragmentShader == 0) return;

		newProgramHandle = glCreateProgram();
		glAttachShader(newProgramHandle, vertexShader);
		glAttachShader(newProgramHandle, fragmentShader);
		glLinkProgram(newProgramHandle);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	GLint isLinked = 0;
	glGetProgramiv(newProgramHandle, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(newProgramHandle, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(newProgramHandle, maxLength, &maxLength, &infoLog[0]);
		Log::Error("OpenGLShader::Reload - Shader linking error: " + std::string(infoLog.data()));
		glDeleteProgram(newProgramHandle);
		return;
	}

	if (m_Handle) glDeleteProgram(m_Handle);
	m_Handle = newProgramHandle;
}

void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
	GLint location = glGetUniformLocation(m_Handle, name.c_str());
	glProgramUniform1i(m_Handle, location, value);
}

void OpenGLShader::UploadUniformFloat(const std::string& name, float value) {
	GLint location = glGetUniformLocation(m_Handle, name.c_str());
	glProgramUniform1f(m_Handle, location, value);
}

void OpenGLShader::UploadUniformBool(const std::string& name, bool value) {
	GLint location = glGetUniformLocation(m_Handle, name.c_str());
	glProgramUniform1i(m_Handle, location, value);
}

void OpenGLShader::UploadUniformVec2(const std::string& name, const glm::vec2& value) {
	GLint location = glGetUniformLocation(m_Handle, name.c_str());
	glProgramUniform2fv(m_Handle, location, 1, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformVec3(const std::string& name, const glm::vec3& value) {
	GLint location = glGetUniformLocation(m_Handle, name.c_str());
	glProgramUniform3fv(m_Handle, location, 1, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformVec4(const std::string& name, const glm::vec4& value) {
	GLint location = glGetUniformLocation(m_Handle, name.c_str());
	glProgramUniform4fv(m_Handle, location, 1, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value) {
	GLint location = glGetUniformLocation(m_Handle, name.c_str());
	glProgramUniformMatrix4fv(m_Handle, location, 1, GL_FALSE, glm::value_ptr(value));
}

GLuint OpenGLShader::CompileShader(GLenum type, const std::string& source) {
	std::string typeStr = ShaderTypeToString(type);
	Log::Trace("OpenGLShader::CompileShader - Compiling OpenGL " + typeStr + " Shader");

	GLuint shader = glCreateShader(type);
	const GLchar* sourceCStr = source.c_str();
	glShaderSource(shader, 1, &sourceCStr, 0);
	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
		Log::Error("Shader::CompileShader - " + typeStr + " Shader compilation error: " + std::string(infoLog.data()));
		glDeleteShader(shader);
		return 0;
	}

	Log::Trace("OpenGLShader::CompileShader - OpenGL " + typeStr + " Shader compiled");
	return shader;
}

std::string OpenGLShader::ReadTextFile(const std::filesystem::path& path) {
	if (path.empty()) {
		Log::Error("Shader::ReadTextFile - File is empty: " + path.string());
		return "";
	}

	Log::Trace("OpenGLShader::ReadTextFile - Reading Text File: " + path.string());

	std::ifstream file(path);

	if (!file.is_open()) {
		Log::Error("Shader::ReadTextFile - Failed to open file: " + path.string());
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

void OpenGLShader::Bind() const {
	glUseProgram(m_Handle);
}

void OpenGLShader::Unbind() const {
	glUseProgram(0);
}