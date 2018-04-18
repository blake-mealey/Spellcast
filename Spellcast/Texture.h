#pragma once

#include <GL/glew.h>

#include <memory>
#include <string>

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class Texture {
public:
	Texture();
	~Texture();

	static TexturePtr Create();

	bool LoadFromFile(const std::string& a_filePath);
	void Bind(GLuint a_textureUnit) const;
private:
	GLuint m_texture;
	int m_width;
	int m_height;
};
