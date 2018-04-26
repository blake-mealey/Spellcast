#pragma once

#include <GL/glew.h>

#include <string>

#define INVALID_TEXTURE_PARAM -1

class TextureDesc {
public:
	TextureDesc();
	TextureDesc(GLsizei a_width, GLsizei a_height, const void* a_pixels);

	GLsizei m_width;
	GLsizei m_height;
	
	GLint m_internalFormat;
	GLint m_format;
	GLenum m_type;

	GLint m_wrapS;
	GLint m_wrapT;
	
	GLint m_minFilter;
	GLint m_magFilter;

	GLint m_compareFunc;
	GLint m_compareMode;

	const void* m_pixels;
};

class Texture {
public:
	Texture();
	~Texture();

	bool LoadFromFile(const std::string& a_filePath);

	bool Init(const TextureDesc& a_desc);
	
	bool UpdateDimensions(int a_width, int a_height);

	void Bind(GLenum a_textureUnit) const;
	GLuint GetId() const;
private:
	static void SetParameter(GLenum a_parameter, GLint a_value);

	GLuint m_texture;
	GLint m_format;
	int m_width;
	int m_height;
};
