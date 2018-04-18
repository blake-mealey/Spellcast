#include "Mesh.h"
#include "MeshRenderer.h"

#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

using namespace std;
using namespace glm;
using namespace Assimp;

Mesh::Mesh() = default;
Mesh::~Mesh() = default;

MeshPtr Mesh::Create() {
	return make_shared<Mesh>();
}

bool Mesh::LoadFromFile(const std::string& a_filePath, const unsigned a_flags) {
	Importer importer;

	const aiScene* scene = importer.ReadFile(a_filePath,
		a_flags
		| aiProcess_CalcTangentSpace
		| aiProcess_Triangulate
		| aiProcess_GenSmoothNormals
		| aiProcess_FlipUVs
		| aiProcess_JoinIdenticalVertices
	);

	if (!scene) {
		cerr << "WARNING: Error parsing " << a_filePath << ": " << importer.GetErrorString() << endl;
		return false;
	}

	Init(scene);
	return true;
}

void Mesh::Render(const MeshRenderer* a_context) const {
	for (const MeshEntry& entry : m_entries) {
		entry.Render(a_context);
	}
}

void Mesh::Init(const aiScene* a_scene) {
	m_entries.resize(a_scene->mNumMeshes);

	for (size_t i = 0; i < m_entries.size(); ++i) {
		const aiMesh* aiMesh = a_scene->mMeshes[i];

		std::vector<t_index> indices;
		std::vector<vec3> vertices;
		std::vector<vec2> uvs;
		std::vector<vec3> normals;

		for (size_t j = 0; j < aiMesh->mNumFaces; ++j) {
			const aiFace& face = aiMesh->mFaces[j];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		const aiVector3D zero3D(0.f, 0.f, 0.f);
		for (size_t j = 0; j < aiMesh->mNumVertices; ++j) {
			const aiVector3D& vertex = aiMesh->mVertices[j];
			const aiVector3D& normal = aiMesh->HasNormals() ? aiMesh->mNormals[j] : zero3D;
			const aiVector3D& uv = aiMesh->HasTextureCoords(0) ? aiMesh->mTextureCoords[0][j] : zero3D;

			vertices.emplace_back(vertex.x, vertex.y, vertex.z);
			normals.emplace_back(normal.x, normal.y, normal.z);
			uvs.emplace_back(uv.x, uv.y);
		}

		m_entries[i].Init(indices, vertices, uvs, normals);
	}
}



Mesh::MeshEntry::MeshEntry(): m_indexBuffer(0), m_vaos{}, m_vbos{}, m_indexCount(0), m_vertexCount(0),
                              m_materialIndex(0) { }

Mesh::MeshEntry::~MeshEntry() {
	if (m_indexBuffer) {
		glDeleteBuffers(1, &m_indexBuffer);
		glDeleteBuffers(VBOs::Count, m_vbos);
		glDeleteVertexArrays(VAOs::Count, m_vaos);
	}
}

bool Mesh::MeshEntry::Init(const vector<t_index>& indices, const vector<glm::vec3>& vertices,
	const vector<vec2>& uvs, const vector<vec3>& normals) {

	// Initialize counts
	m_indexCount = indices.size();
	m_vertexCount = vertices.size();


	// Initialize index buffer
	glGenBuffers(1, &m_indexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(t_index) * m_indexCount, indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// Initialize geometry buffers
	glGenBuffers(VBOs::Count, m_vbos);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VBOs::Vertices]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertexCount, vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VBOs::Uvs]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_vertexCount, uvs.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VBOs::Normals]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertexCount, normals.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Initialize vertex arrays
	glGenVertexArrays(VAOs::Count, m_vaos);
	glBindVertexArray(m_vaos[VAOs::Geometry]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VBOs::Vertices]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VBOs::Uvs]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VBOs::Normals]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));

	glBindVertexArray(0);

	return glGetError() == GL_NO_ERROR;
}

void Mesh::MeshEntry::Render(const MeshRenderer* a_context) const {
	// Use the current context's material
	a_context->GetMaterial(m_materialIndex)->Use();
	// TODO: Load the correct matrices to the shader as well

	glBindVertexArray(m_vaos[VAOs::Geometry]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
}
