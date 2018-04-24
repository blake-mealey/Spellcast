#include "Texture.h"
#include "ContentManager.h"
#include "Image.h"

using namespace std;

Texture::Texture(): m_texture(0), m_width(0), m_height(0) {}

Texture::~Texture() {
	if (m_texture) {
		glDeleteTextures(1, &m_texture);
	}
}

bool Texture::LoadFromFile(const std::string& a_filePath) {
	// Load file
	Image image;
	if (!image.LoadFromFile(a_filePath)) return false;

	// Store dimensions
	m_width = image.GetWidth();
	m_height = image.GetHeight();

	// Generate texture
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Add image to texture
	const GLint format = image.GetFormat();
	if (format > -1) glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, image.GetData());

	// Initialize texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Unbind texture and return true
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void Texture::Bind(const GLenum a_textureUnit) const {
	glActiveTexture(a_textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
