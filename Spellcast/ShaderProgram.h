#pragma once

#include "ContentManager.h"

#include <GL/glew.h>

#include <string>
#include <vector>
#include <memory>

class ShaderProgram;
typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;

class ShaderProgram {
public:
	ShaderProgram();
	virtual ~ShaderProgram();

	static ShaderProgramPtr Create();

	virtual bool Init();
	void Enable() const;
	static void Disable();
	
	virtual void SetMaterial(const Material* a_material) {};

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
	void DeleteShaders();

	std::vector<GLuint> m_shaders;
};
