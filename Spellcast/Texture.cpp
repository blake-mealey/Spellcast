#include "Texture.h"
#include "ContentManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

using namespace std;

Texture::Texture(): m_texture(0), m_width(0), m_height(0) {}

Texture::~Texture() {
	if (m_texture) {
		glDeleteTextures(1, &m_texture);
	}
}

TexturePtr Texture::Create() {
	return make_shared<Texture>();
}

bool Texture::LoadFromFile(const std::string& a_filePath) {
	int components;
	const auto data = stbi_load(a_filePath.c_str(), &m_width, &m_height, &components, 0);

	if (!data) {
		cerr << "WARNING: Failed to load texture: " << a_filePath << endl;
		return false;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	const GLint format = (components == 3) ? GL_RGB : (components == 4) ? GL_RGBA : -1;
	if (format > -1) glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	return true;
}

void Texture::Bind(const GLuint a_textureUnit) const {
	glActiveTexture(a_textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
