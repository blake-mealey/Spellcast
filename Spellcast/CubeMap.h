#pragma once

#include "Image.h"

#include <GL/glew.h>

#include <string>
#include <array>

#define CUBE_MAP_FACE_COUNT 6

#define INVALID_CUBE_MAP_PARAM -1

class CubeMapDesc {
public:
	CubeMapDesc();
	
	GLenum m_type;
	
	GLint m_wrapS;
	GLint m_wrapT;
	GLint m_wrapR;
	
	GLint m_minFilter;
	GLint m_magFilter;

	std::array<Image, CUBE_MAP_FACE_COUNT> m_images;
};

class CubeMap {
public:
	CubeMap();
	~CubeMap();

	bool LoadFromFile(const std::string& a_dirPath);
	bool Init(const CubeMapDesc& a_desc);

	void Bind(GLenum a_textureUnit) const;

	static const std::string FACE_NAMES[6];
private:
	static void SetParameter(GLenum a_parameter, GLint a_value);

	GLuint m_cubeMap;
	int m_width;
	int m_height;
	int m_depth;
};
