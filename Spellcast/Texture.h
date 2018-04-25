#pragma once

#include <GL/glew.h>

#include <string>

class Texture {
public:
	Texture();
	~Texture();

	bool LoadFromFile(const std::string& a_filePath);
	
	bool Init(GLint a_format, int a_width, int a_height, const void* a_pixels, GLint a_wrapMode = GL_REPEAT);
	bool Init(GLint a_internalFormat, GLint a_format, GLenum a_type, int a_width, int a_height, const void* a_pixels, GLint a_wrapMode = GL_REPEAT);
	
	void SetParameter(GLenum a_parameter, GLint a_value) const;
	bool UpdateDimensions(int a_width, int a_height);

	void Bind(GLenum a_textureUnit) const;
	GLuint GetId() const;
private:
	GLuint m_texture;
	GLint m_format;
	int m_width;
	int m_height;
};
