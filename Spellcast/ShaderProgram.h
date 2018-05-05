#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <string>
#include <vector>
#include <regex>

class Material;

typedef GLint uniform_loc;

struct ShaderSource {
	ShaderSource(): m_valid(false), m_lines(0), m_start(0) {}

	bool m_valid;
	
	std::string m_resolvedFilePath;
	
	size_t m_lines;
	size_t m_start;
	std::string m_source;
	
	std::vector<ShaderSource> m_includes;
};

class ShaderProgram {
public:
	ShaderProgram();
	virtual ~ShaderProgram();

	virtual bool Init();
	void Enable() const;
	static void Disable();
	
	virtual void SetMaterial(const Material* a_material) const {}
	virtual void SetModelAndViewAndProjectionMatrices(const glm::mat4& a_modelMatrix, const glm::mat4& a_viewMatrix, const glm::mat4& a_projectionMatrix) const {}

protected:
	bool AddShader(GLenum a_shaderType, const std::string& a_filePath);
	bool Finalize();

	uniform_loc GetUniformLocation(const std::string& a_uniformName);
	GLint GetProgramParam(GLint a_param) const;
	
	GLuint m_program;
	bool m_invalidUniform;

	static void LoadUniform(const uniform_loc& a_location, const bool& a_value);
	static void LoadUniform(const uniform_loc& a_location, const GLuint& a_value);
	static void LoadUniform(const uniform_loc& a_location, const int& a_value);
	static void LoadUniform(const uniform_loc& a_location, const float& a_value);
	static void LoadUniform(const uniform_loc& a_location, const glm::vec2& a_value);
	static void LoadUniform(const uniform_loc& a_location, const glm::vec3& a_value);
	static void LoadUniform(const uniform_loc& a_location, const glm::vec4& a_value);
	static void LoadUniform(const uniform_loc& a_location, const glm::mat4& a_value);

private:
	static const std::regex DIRECTIVE_REGEX;
	static const std::regex INCLUDE_REGEX;
	static const std::regex LINE_NUMBER_REGEX;

	static const ShaderSource& GetShaderSourceByLineNumber(const ShaderSource& a_shaderSource, size_t a_lineNumber);
	ShaderSource GetShaderSource(const std::string& a_filePath) const;
	
	void DeleteShaders();

	std::vector<GLuint> m_shaders;
};
