#include "ArrayTexture.h"
#include "Logger.h"

ArrayTextureDesc::ArrayTextureDesc(): m_width(0), m_height(0), m_maxSize(0), m_internalFormat(GL_RGBA),
                                      m_wrapS(GL_CLAMP_TO_EDGE),
                                      m_wrapT(GL_CLAMP_TO_EDGE),
                                      m_minFilter(GL_LINEAR), m_magFilter(GL_LINEAR),
                                      m_compareFunc(INVALID_ARRAY_TEXTURE_PARAM),
                                      m_compareMode(INVALID_ARRAY_TEXTURE_PARAM) {}

ArraySubTextureDesc::ArraySubTextureDesc(): m_format(GL_RGBA), m_type(GL_UNSIGNED_BYTE), m_pixels(nullptr) {}




ArrayTexture::ArrayTexture(): m_arrayTexture(0), m_width(0), m_height(0), m_maxSize(0), m_size(0) {}

ArrayTexture::~ArrayTexture() {
	if (m_arrayTexture) glDeleteTextures(1, &m_arrayTexture);
}

bool ArrayTexture::Init(const ArrayTextureDesc& a_desc) {
	// Store metadata
	m_width = a_desc.m_width;
	m_height = a_desc.m_height;
	m_maxSize = a_desc.m_maxSize;

	// Error check
	if (m_width < 1 || m_height < 1) {
		Logger::Console()->warn("Invalid dimensions of image.");
		return false;
	}

	// Generate texture
	glGenTextures(1, &m_arrayTexture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_arrayTexture);

	// Set format and dimensions of array texture
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, a_desc.m_internalFormat, m_width, m_height, m_maxSize);

	// Initialize texture
	SetParameter(GL_TEXTURE_WRAP_S, a_desc.m_wrapS);
	SetParameter(GL_TEXTURE_WRAP_T, a_desc.m_wrapT);
	SetParameter(GL_TEXTURE_MIN_FILTER, a_desc.m_minFilter);
	SetParameter(GL_TEXTURE_MAG_FILTER, a_desc.m_magFilter);
	SetParameter(GL_TEXTURE_COMPARE_FUNC, a_desc.m_compareFunc);
	SetParameter(GL_TEXTURE_COMPARE_MODE, a_desc.m_compareMode);
	SetParameter(GL_TEXTURE_MAG_FILTER, a_desc.m_magFilter);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	return glGetError() == GL_NO_ERROR;
}

void ArrayTexture::Bind(const GLenum a_textureUnit) const {
	glActiveTexture(a_textureUnit);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_arrayTexture);
}

GLuint ArrayTexture::GetId() const {
	return m_arrayTexture;
}

void ArrayTexture::Clear() {
	m_size = 0;
}

size_t ArrayTexture::GetSize() const {
	return m_size;
}

int ArrayTexture::AddSubTexture(const ArraySubTextureDesc& a_desc) {
	if (m_size == m_maxSize) return false;
	const size_t index = m_size++;		// Intentionally incrementing postfix
	return SetSubTexture(index, a_desc) ? index : -1;
}

bool ArrayTexture::SetSubTexture(const size_t a_index, const ArraySubTextureDesc& a_desc) const {
	if (a_index >= m_size) return false;

	glBindTexture(GL_TEXTURE_2D_ARRAY, m_arrayTexture);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, a_index, m_width, m_height, 1, a_desc.m_format, a_desc.m_type,
	                a_desc.m_pixels);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	return glGetError() == GL_NO_ERROR;
}

void ArrayTexture::SetParameter(const GLenum a_parameter, const GLint a_value) {
	if (a_value != INVALID_ARRAY_TEXTURE_PARAM) glTexParameteri(GL_TEXTURE_2D_ARRAY, a_parameter, a_value);
}
