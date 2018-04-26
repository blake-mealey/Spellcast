#include "Texture.h"
#include "Image.h"
#include "Logger.h"

using namespace std;

TextureDesc::TextureDesc(): m_width(0), m_height(0), m_internalFormat(GL_RGBA), m_format(GL_RGBA),
                            m_type(GL_UNSIGNED_BYTE),
                            m_wrapS(GL_REPEAT), m_wrapT(GL_REPEAT), m_minFilter(GL_LINEAR), m_magFilter(GL_LINEAR),
                            m_compareFunc(INVALID_TEXTURE_PARAM),
                            m_compareMode(INVALID_TEXTURE_PARAM), m_pixels(nullptr) { }

TextureDesc::TextureDesc(GLsizei a_width, GLsizei a_height, const void* a_pixels) : TextureDesc() {
	m_width = a_width;
	m_height = a_height;
	m_pixels = a_pixels;
}





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

	// Create description and initialize
	TextureDesc desc(image.GetWidth(), image.GetHeight(), image.GetData());
	desc.m_internalFormat = image.GetFormat();
	desc.m_format = image.GetFormat();
	return Init(desc);
}

bool Texture::Init(const TextureDesc& a_desc) {
	// Store metadata
	m_width = a_desc.m_width;
	m_height = a_desc.m_height;
	m_format = a_desc.m_format;

	// Error check
	if (m_width < 1 || m_height < 1) {
		Logger::Console()->warn("Invalid dimensions of image.");
		return false;
	}

	// Generate texture
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Set format, dimensions, and data of texture
	glTexImage2D(GL_TEXTURE_2D, 0, a_desc.m_internalFormat, m_width, m_height, 0, m_format, a_desc.m_type, a_desc.m_pixels);

	// Initialize texture
	SetParameter(GL_TEXTURE_WRAP_S, a_desc.m_wrapS);
	SetParameter(GL_TEXTURE_WRAP_T, a_desc.m_wrapT);
	SetParameter(GL_TEXTURE_MIN_FILTER, a_desc.m_minFilter);
	SetParameter(GL_TEXTURE_MAG_FILTER, a_desc.m_magFilter);
	SetParameter(GL_TEXTURE_COMPARE_FUNC, a_desc.m_compareFunc);
	SetParameter(GL_TEXTURE_COMPARE_MODE, a_desc.m_compareMode);

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

void Texture::SetParameter(const GLenum a_parameter, const GLint a_value) {
	if (a_value != INVALID_TEXTURE_PARAM) glTexParameteri(GL_TEXTURE_2D, a_parameter, a_value);
}
