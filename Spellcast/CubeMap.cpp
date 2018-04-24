#include "CubeMap.h"
#include "Image.h"
#include "ContentManager.h"

using namespace std;

#define FACE_COUNT 6

const string CubeMap::FACE_NAMES[FACE_COUNT] = { "right", "left", "top", "bottom", "front", "back" };

CubeMap::CubeMap(): m_cubeMap(0), m_width(0), m_height(0), m_depth(0) {}

CubeMap::~CubeMap() {
	if (m_cubeMap) {
		glDeleteTextures(1, &m_cubeMap);
	}
}

bool CubeMap::LoadFromFile(const std::string& a_dirPath) {
	// Load images
	Image images[FACE_COUNT];
	for (size_t i = 0; i < FACE_COUNT; ++i)
		if (!images[i].LoadFromFile(a_dirPath + FACE_NAMES[i] + ".png")) return false;

	// Store dimensions (TODO: Check these)
	m_height = images[0].GetHeight();
	m_width = images[2].GetWidth();
	m_depth = images[0].GetWidth();

	// Generate cube map
	glGenTextures(1, &m_cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);

	// Add images to cube map
	for (size_t i = 0; i < FACE_COUNT; ++i) {
		Image& image = images[i];
		const GLint format = image.GetFormat();
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format,
			image.GetWidth(), image.GetHeight(), 0, format, GL_UNSIGNED_BYTE, image.GetData());
	}

	// Initialize cube map
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Unbind texture and return true
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return true;
}

void CubeMap::Bind(const GLenum a_textureUnit) const {
	glActiveTexture(a_textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
}
