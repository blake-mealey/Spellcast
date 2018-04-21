#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <string>
#include <vector>
#include <regex>

class Material;

class ShaderProgram {
public:
	ShaderProgram();
	virtual ~ShaderProgram();

	virtual bool Init();
	void Enable() const;
	static void Disable();
	
	virtual void SetMaterial(const Material* a_material) const {};
	virtual void SetModelAndViewAndProjectionMatrices(const glm::mat4& a_modelMatrix, const glm::mat4& a_viewMatrix, const glm::mat4& a_projectionMatrix) const {};

protected:
	bool AddShader(GLenum a_shaderType, const std::string& a_filePath);
	bool Finalize();

	GLint GetUniformLocation(const std::string& a_uniformName);
	GLint GetProgramParam(GLint a_param) const;
	
	GLuint m_program;
	bool m_invalidUniform;

	static void LoadUniform(const GLint& a_location, const bool& a_value);
	static void LoadUniform(const GLint& a_location, const GLuint& a_value);
	static void LoadUniform(const GLint& a_location, const int& a_value);
	static void LoadUniform(const GLint& a_location, const float& a_value);
	static void LoadUniform(const GLint& a_location, const glm::vec2& a_value);
	static void LoadUniform(const GLint& a_location, const glm::vec3& a_value);
	static void LoadUniform(const GLint& a_location, const glm::vec4& a_value);
	static void LoadUniform(const GLint& a_location, const glm::mat4& a_value);

private:
	static const std::regex S_NEW_LINE;
	static const std::regex S_DIRECTIVE;
	static const std::regex S_INCLUDE;
	
	bool PreprocessShaderSource(const std::string& a_filePath, std::string& a_source, std::string& a_resolvedFilePath) const;
	bool PreprocessShaderSource(const std::string& a_filePath, std::string& a_source) const;
	static void FlattenSource(std::string& a_source);

	void DeleteShaders();

	std::vector<GLuint> m_shaders;
};
