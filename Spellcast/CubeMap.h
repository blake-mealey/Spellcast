#pragma once

#include <GL/glew.h>

#include <string>

class CubeMap {
public:
	CubeMap();
	~CubeMap();

	bool LoadFromFile(const std::string& a_dirPath);
	void Bind(GLenum a_textureUnit) const;

	static const std::string FACE_NAMES[6];
private:
	GLuint m_cubeMap;
	int m_width;
	int m_height;
	int m_depth;
};
