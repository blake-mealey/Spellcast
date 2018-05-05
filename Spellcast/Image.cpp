#include "Image.h"
#include "Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace std;

Image::Image() : m_data(nullptr), m_componentCount(0), m_width(0), m_height(0), m_fromStb(false) { }

Image::~Image() {
	if (m_data) {
		if (m_fromStb) stbi_image_free(m_data);
		else delete[] m_data;
	}
}

bool Image::LoadFromFile(const std::string& a_filePath) {
	m_data = stbi_load(a_filePath.c_str(), &m_width, &m_height, &m_componentCount, 0);

	if (!m_data) {
		Logger::Console()->warn("Failed to load image: {}", a_filePath);
		return false;
	}
	
	m_fromStb = true;

	return true;
}

Image* Image::GetSubImage(const size_t a_x, const size_t a_y, const size_t a_width, const size_t a_height) const {
	if (a_x >= m_width || a_y >= m_height || a_width > m_width || a_height > m_height) return nullptr;

	auto* copy = new Image();

	copy->m_width = a_width;
	copy->m_height = a_height;
	copy->m_componentCount = m_componentCount;

	copy->m_data = new pixel_t[a_width * a_height * m_componentCount];

	pixel_t* src = m_data + (m_componentCount * (a_y*m_width + a_x));
	pixel_t* dst = copy->m_data;

	const size_t dstOffset = a_width * m_componentCount;
	const size_t srcOffset = m_width * m_componentCount;
	for (size_t i = 0; i < a_height; ++i) {
		memcpy(dst, src, dstOffset);
		dst += dstOffset;
		src += srcOffset;
	}

	return copy;
}

pixel_t* Image::GetData() const {
	return m_data;
}

GLint Image::GetFormat() const {
	return (m_componentCount == 3) ? GL_RGB : (m_componentCount == 4) ? GL_RGBA : -1;
}

int Image::GetWidth() const {
	return m_width;
}

int Image::GetHeight() const {
	return m_height;
}
