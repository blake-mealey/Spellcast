#include "CubeChunk.h"

#include <algorithm>

using namespace std;
using namespace glm;

CubeChunk::~CubeChunk() {
	if (m_vao) glDeleteVertexArrays(1, &m_vao);
	if (m_vertices) glDeleteBuffers(2, &m_vertices);
}

CubeChunk::CubeChunk(): m_chunk{}, m_vao(0), m_vertices(0), m_normals(0), m_elements(0), m_changed(false) {}

bool CubeChunk::Init() {
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(2, &m_vertices);

	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
	glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, 0, nullptr);
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_normals);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

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

	auto* vertices = new byte4[CX * CY * CZ * 6 * 6];
	auto* normals = new vec3[CX * CY * CZ * 6 * 6];
	int i = 0;

	for (int x = 0; x < CX; ++x) {
		for (int y = 0; y < CX; ++y) {
			for (int z = 0; z < CX; ++z) {
				const cube_t type = m_chunk[x][y][z];
				if (!type) continue;

				// view from negative x
				if (x == 0 || !m_chunk[x - 1][y][z]) {
					const vec3 normal = vec3(-1.f, 0.f, 0.f);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y,     z,     type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y,     z + 1, type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y + 1, z,     type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y + 1, z,     type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y,     z + 1, type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y + 1, z + 1, type);
				}

				// view from positive x
				if (x == CX - 1 || !m_chunk[x + 1][y][z]) {
					const vec3 normal = vec3(1.f, 0.f, 0.f);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y,     z,     type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y + 1, z,     type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y,     z + 1, type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y + 1, z,     type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y + 1, z + 1, type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y,     z + 1, type);
				}

				// view from negative z
				if (z == 0 || !m_chunk[x][y][z - 1]) {
					const vec3 normal = vec3(0.f, 0.f, -1.f);
					vertices[i++] = byte4(x + 1, y,     z,     type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y,     z,     type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y + 1, z,     type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y + 1, z,     type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y + 1, z,     type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y,     z,     type);
				}

				// view from positive z
				if (z == CZ - 1 || !m_chunk[x][y][z + 1]) {
					const vec3 normal = vec3(0.f, 0.f, 1.f);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y,     z + 1, type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y,     z + 1, type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y + 1, z + 1, type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y + 1, z + 1, type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y,     z + 1, type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y + 1, z + 1, type);
				}

				// view from negative y
				if (y == 0 || !m_chunk[x][y - 1][z]) {
					const vec3 normal = vec3(0.f, -1.f, 0.f);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y,     z,     -type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y,     z,     -type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y,     z + 1, -type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y,     z + 1, -type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y,     z,     -type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y,     z + 1, -type);
				}

				// view from positive y
				if (y == CY - 1 || !m_chunk[x][y + 1][z]) {
					const vec3 normal = vec3(0.f, 1.f, 0.f);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y + 1, z,     -type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y + 1, z + 1, -type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y + 1, z,     -type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y + 1, z,     -type);
					normals[i] = normal;
					vertices[i++] = byte4(x,     y + 1, z + 1, -type);
					normals[i] = normal;
					vertices[i++] = byte4(x + 1, y + 1, z + 1, -type);
				}
			}
		}
	}

	m_elements = i;

	glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
	glBufferData(GL_ARRAY_BUFFER, m_elements * sizeof(byte4), vertices, GL_STATIC_DRAW);
	delete[] vertices;

	glBindBuffer(GL_ARRAY_BUFFER, m_normals);
	glBufferData(GL_ARRAY_BUFFER, m_elements * sizeof(vec3), normals, GL_STATIC_DRAW);
	delete[] normals;

	return glGetError() == GL_NO_ERROR;
}

void CubeChunk::Render() {
	if (m_changed) Update();

	if (!m_elements) return;

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_elements);
}

