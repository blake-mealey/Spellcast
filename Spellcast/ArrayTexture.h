#pragma once

#include <GL/glew.h>

#include <string>

#define INVALID_ARRAY_TEXTURE_PARAM -1

class ArrayTextureDesc {
public:
	ArrayTextureDesc();
	
	GLsizei m_width;
	GLsizei m_height;
	size_t m_maxSize;
	
	GLint m_internalFormat;

	GLint m_wrapS;
	GLint m_wrapT;
	
	GLint m_minFilter;
	GLint m_magFilter;

	GLint m_compareFunc;
	GLint m_compareMode;
};

class ArraySubTextureDesc {
public:
	ArraySubTextureDesc();
	
	GLint m_format;
	GLenum m_type;

	const void* m_pixels;
};

class ArrayTexture {
public:
	ArrayTexture();
	~ArrayTexture();

	bool LoadFromFile(const std::string& a_filePath, bool a_useHeightAsWidth = true);

	bool Init(const ArrayTextureDesc& a_desc);

	void Bind(GLenum a_textureUnit) const;
	GLuint GetId() const;

	void Clear();
	size_t GetSize() const;

	int AddSubTexture(const ArraySubTextureDesc& a_desc);
	bool SetSubTexture(size_t a_index, const ArraySubTextureDesc& a_desc) const;
private:
	static void SetParameter(GLenum a_parameter, GLint a_value);

	GLuint m_arrayTexture;
	int m_width;
	int m_height;
	size_t m_maxSize;

	size_t m_size;
};
