#pragma once

#include <GL/glew.h>

#include <string>

class Texture {
public:
	Texture();
	~Texture();

	bool LoadFromFile(const std::string& a_filePath);
	void Bind(GLuint a_textureUnit) const;
private:
	GLuint m_texture;
	int m_width;
	int m_height;
};
