#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <string>
#include <vector>

class ShaderProgram {
public:
	ShaderProgram();
	virtual ~ShaderProgram();

	virtual bool Init();
	void Enable() const;
	static void Disable();

protected:
	bool AddShader(GLenum a_shaderType, const std::string& a_filePath);
	bool Finalize();

	GLint GetUniformLocation(const std::string& a_uniformName);
	GLint GetProgramParam(GLint a_param) const;
	
	GLuint m_shaderProgram;
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
	void DeleteShaders();

	std::vector<GLuint> m_shaders;
};
