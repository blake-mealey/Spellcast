#include "CubeMap.h"
#include "Image.h"
#include "ContentManager.h"

using namespace std;

CubeMapDesc::CubeMapDesc(): m_type(GL_UNSIGNED_BYTE), m_wrapS(GL_CLAMP_TO_EDGE), m_wrapT(GL_CLAMP_TO_EDGE),
                            m_wrapR(GL_CLAMP_TO_EDGE),
                            m_minFilter(GL_LINEAR), m_magFilter(GL_LINEAR) {}


const string CubeMap::FACE_NAMES[CUBE_MAP_FACE_COUNT] = { "right", "left", "top", "bottom", "front", "back" };

CubeMap::CubeMap(): m_cubeMap(0), m_width(0), m_height(0), m_depth(0) {}

CubeMap::~CubeMap() {
	if (m_cubeMap) {
		glDeleteTextures(1, &m_cubeMap);
	}
}

bool CubeMap::LoadFromFile(const std::string& a_dirPath) {
	// Load images
	CubeMapDesc desc;
	for (size_t i = 0; i < CUBE_MAP_FACE_COUNT; ++i)
		if (!desc.m_images[i].LoadFromFile(a_dirPath + FACE_NAMES[i] + ".png")) return false;

	return Init(desc);
}

bool CubeMap::Init(const CubeMapDesc& a_desc) {
	// Store dimensions (TODO: Check these)
	m_height = a_desc.m_images[0].GetHeight();
	m_width = a_desc.m_images[2].GetWidth();
	m_depth = a_desc.m_images[0].GetWidth();

	// Generate cube map
	glGenTextures(1, &m_cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);

	// Add images to cube map
	for (size_t i = 0; i < CUBE_MAP_FACE_COUNT; ++i) {
		const Image& image = a_desc.m_images[i];
		const GLint format = image.GetFormat();
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format,
			image.GetWidth(), image.GetHeight(), 0, format, a_desc.m_type, image.GetData());
	}

	// Initialize cube map
	SetParameter(GL_TEXTURE_WRAP_S, a_desc.m_wrapS);
	SetParameter(GL_TEXTURE_WRAP_T, a_desc.m_wrapT);
	SetParameter(GL_TEXTURE_WRAP_R, a_desc.m_wrapR);
	SetParameter(GL_TEXTURE_MIN_FILTER, a_desc.m_minFilter);
	SetParameter(GL_TEXTURE_MAG_FILTER, a_desc.m_magFilter);

	// Unbind texture and return true
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return glGetError() == GL_NO_ERROR;
}

void CubeMap::Bind(const GLenum a_textureUnit) const {
	glActiveTexture(a_textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
}

void CubeMap::SetParameter(const GLenum a_parameter, const GLint a_value) {
	if (a_value != INVALID_CUBE_MAP_PARAM) glTexParameteri(GL_TEXTURE_CUBE_MAP, a_parameter, a_value);
}
