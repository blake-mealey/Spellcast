#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <cstdint>

#define CX 64
#define CY 64
#define CZ 64

typedef uint8_t cube_t;
typedef glm::tvec4<GLbyte> byte4;

// Based on the tutorials from: https://en.wikibooks.org/wiki/OpenGL_Programming/Glescraft_1

class CubeChunk {
public:
	~CubeChunk();
	CubeChunk();

	bool Init();

	cube_t Get(int a_x, int a_y, int a_z) const;
	void Set(int a_x, int a_y, int a_z, cube_t a_type);

	bool Update();
	void Render();

private:
	cube_t m_chunk[CX][CY][CZ];
	GLuint m_vao;
	GLuint m_vertices;
	GLuint m_normals;
	int m_elements;
	bool m_changed;
};