#pragma once

#include "Core/Core.h"

#include <filesystem>
#include <glm/glm.hpp>

class Shader {
public:
	virtual ~Shader() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual void Reload() = 0;

	virtual const std::filesystem::path& GetFilepath() const = 0;

	template<typename T>
	void SetUniform(const std::string& name, const T& value) {
		if constexpr (std::is_same_v<T, int>) {
			UploadUniformInt(name, value);
		} else if constexpr (std::is_same_v<T, float>) {
			UploadUniformFloat(name, value);
		} else if constexpr (std::is_same_v<T, bool>) {
			UploadUniformBool(name, value);
		} else if constexpr (std::is_same_v<T, glm::vec2>) {
			UploadUniformVec2(name, value);
		} else if constexpr (std::is_same_v<T, glm::vec3>) {
			UploadUniformVec3(name, value);
		} else if constexpr (std::is_same_v<T, glm::vec4>) {
			UploadUniformVec4(name, value);
		} else if constexpr (std::is_same_v<T, glm::mat4>) {
			UploadUniformMat4(name, value);
		}
	}

	static Ref<Shader> Create(const std::filesystem::path& shaderAssetPath);

	static Ref<Shader> CreateCompute(const std::filesystem::path& path);
	static Ref<Shader> CreateGraphics(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
private:
	virtual void UploadUniformInt(const std::string& name, int value) = 0;
	virtual void UploadUniformFloat(const std::string& name, float value) = 0;
	virtual void UploadUniformBool(const std::string& name, bool value) = 0;
	virtual void UploadUniformVec2(const std::string& name, const glm::vec2& value) = 0;
	virtual void UploadUniformVec3(const std::string& name, const glm::vec3& value) = 0;
	virtual void UploadUniformVec4(const std::string& name, const glm::vec4& value) = 0;
	virtual void UploadUniformMat4(const std::string& name, const glm::mat4& value) = 0;
};