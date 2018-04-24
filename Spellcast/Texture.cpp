#include "Texture.h"
#include "ContentManager.h"
#include "Image.h"
#include "Logger.h"

using namespace std;

Texture::Texture(): m_texture(0), m_format(0), m_width(0), m_height(0) {}

Texture::~Texture() {
	if (m_texture) {
		glDeleteTextures(1, &m_texture);
	}
}

bool Texture::LoadFromFile(const std::string& a_filePath) {
	// Load file
	Image image;
	if (!image.LoadFromFile(a_filePath)) return false;

	// Add image to texture
	const GLint format = image.GetFormat();
	if (format < 0) {
		Logger::Console()->warn("Invalid format of image.");
		return false;
	}

	return Init(image.GetFormat(), image.GetWidth(), image.GetHeight(), image.GetData());
}

bool Texture::Init(const GLint a_format, const int a_width, const int a_height, const void* a_pixels,
                   const GLint a_wrapMode) {
	// Error check
	if (a_format < 0 || a_width < 0 || a_height < 0) {
		Logger::Console()->warn("Invalid format or dimensions of image.");
		return false;
	}

	// Store metadata
	m_width = a_width;
	m_height = a_height;
	m_format = a_format;

	// Generate texture
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Set format, dimensions, and data of texture
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, a_pixels);

	// Initialize texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, a_wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, a_wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}

bool Texture::UpdateDimensions(const int a_width, const int a_height) {
	// Error check
	if (a_width < 0 || a_height < 0) {
		Logger::Console()->warn("Invalid dimensions of image.");
		return false;
	}

	// Store metadata
	m_width = a_width;
	m_height = a_height;

	// Update dimensions (NOTE: Assumes nullptr data)
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}

void Texture::Bind(const GLenum a_textureUnit) const {
	glActiveTexture(a_textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

GLuint Texture::GetId() const {
	return m_texture;
}
