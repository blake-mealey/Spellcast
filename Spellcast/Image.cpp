#include "Image.h"
#include "Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace std;

Image::Image() : m_data(nullptr), m_componentCount(0), m_width(0), m_height(0) { }

Image::~Image() {
	if (m_data) {
		stbi_image_free(m_data);
	}
}

bool Image::LoadFromFile(const std::string& a_filePath) {
	m_data = stbi_load(a_filePath.c_str(), &m_width, &m_height, &m_componentCount, 0);

	if (!m_data) {
		Logger::Console()->warn("Failed to load image: {}", a_filePath);
		return false;
	}

	return true;
}

stbi_uc* Image::GetData() const {
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
