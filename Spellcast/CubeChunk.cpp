#include "CubeChunk.h"

#include <algorithm>

using namespace std;

CubeChunk::~CubeChunk() {
	if (m_vbo) glDeleteBuffers(1, &m_vbo);
}

CubeChunk::CubeChunk(): m_chunk{}, m_vao(0), m_vbo(0), m_elements(0), m_changed(false) {}

bool CubeChunk::Init() {
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, 0, nullptr);

	memset(m_chunk, 0, sizeof(m_chunk));
	m_changed = true;
	
	return glGetError() == GL_NO_ERROR;
}

cube_t CubeChunk::Get(const int a_x, const int a_y, const int a_z) const {
	return m_chunk[a_x][a_y][a_z];
}

void CubeChunk::Set(const int a_x, const int a_y, const int a_z, const cube_t a_type) {
	m_changed = true;
	m_chunk[a_x][a_y][a_z] = a_type;
}

bool CubeChunk::Update() {
	m_changed = false;

	byte4 vertices[CX * CY * CZ * 6 * 6];
	int i = 0;

	for (int x = 0; x < CX; ++x) {
		for (int y = 0; y < CX; ++y) {
			for (int z = 0; z < CX; ++z) {
				const cube_t type = m_chunk[x][y][z];
				if (!type) continue;

				// view from negative x
				if (x == 0 || !m_chunk[x - 1][y][z]) {
					vertices[i++] = byte4(x,     y,     z,     type);
					vertices[i++] = byte4(x,     y,     z + 1, type);
					vertices[i++] = byte4(x,     y + 1, z,     type);
					vertices[i++] = byte4(x,     y + 1, z,     type);
					vertices[i++] = byte4(x,     y,     z + 1, type);
					vertices[i++] = byte4(x,     y + 1, z + 1, type);
				}

				// view from positive x
				if (x == CX - 1 || !m_chunk[x + 1][y][z]) {
					vertices[i++] = byte4(x + 1, y,     z,     type);
					vertices[i++] = byte4(x + 1, y + 1, z,     type);
					vertices[i++] = byte4(x + 1, y,     z + 1, type);
					vertices[i++] = byte4(x + 1, y + 1, z,     type);
					vertices[i++] = byte4(x + 1, y + 1, z + 1, type);
					vertices[i++] = byte4(x + 1, y,     z + 1, type);
				}

				// view from negative z
				if (z == 0 || !m_chunk[x][y][z - 1]) {
					vertices[i++] = byte4(x + 1, y,     z,     type);
					vertices[i++] = byte4(x,     y,     z,     type);
					vertices[i++] = byte4(x,     y + 1, z,     type);
					vertices[i++] = byte4(x,     y + 1, z,     type);
					vertices[i++] = byte4(x + 1, y + 1, z,     type);
					vertices[i++] = byte4(x + 1, y,     z,     type);
				}

				// view from positive z
				if (z == CZ - 1 || !m_chunk[x][y][z + 1]) {
					vertices[i++] = byte4(x,     y,     z + 1, type);
					vertices[i++] = byte4(x + 1, y,     z + 1, type);
					vertices[i++] = byte4(x,     y + 1, z + 1, type);
					vertices[i++] = byte4(x,     y + 1, z + 1, type);
					vertices[i++] = byte4(x + 1, y,     z + 1, type);
					vertices[i++] = byte4(x + 1, y + 1, z + 1, type);
				}

				// view from negative y
				if (y == 0 || !m_chunk[x][y - 1][z]) {
					vertices[i++] = byte4(x,     y,     z,     type);
					vertices[i++] = byte4(x + 1, y,     z,     type);
					vertices[i++] = byte4(x,     y,     z + 1, type);
					vertices[i++] = byte4(x,     y,     z + 1, type);
					vertices[i++] = byte4(x + 1, y,     z,     type);
					vertices[i++] = byte4(x + 1, y,     z + 1, type);
				}

				// view from positive y
				if (y == CY - 1 || !m_chunk[x][y + 1][z]) {
					vertices[i++] = byte4(x,     y + 1, z,     type);
					vertices[i++] = byte4(x,     y + 1, z + 1, type);
					vertices[i++] = byte4(x + 1, y + 1, z,     type);
					vertices[i++] = byte4(x + 1, y + 1, z,     type);
					vertices[i++] = byte4(x,     y + 1, z + 1, type);
					vertices[i++] = byte4(x + 1, y + 1, z + 1, type);
				}
			}
		}
	}

	m_elements = i;
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_elements * sizeof(byte4), vertices, GL_STATIC_DRAW);

	return glGetError() != GL_NO_ERROR;
}

void CubeChunk::Render() {
	if (m_changed) Update();

	if (!m_elements) return;

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_elements);
}

