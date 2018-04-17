#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <memory>

class aiScene;

typedef unsigned int t_index;

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;

class Mesh {
public:
	Mesh();
	~Mesh();

	static MeshPtr Create();

	bool LoadFromFile(const std::string& a_filePath, unsigned a_flags = 0);

	struct VAOs { enum { Geometry = 0, Count }; };

	struct VBOs { enum { Vertices = 0, Normals, Uvs, Count }; };

private:
	void Init(const aiScene* scene);

	typedef unsigned GLuint;
	
	struct MeshEntry {
		MeshEntry();
		~MeshEntry();

		bool Init(const std::vector<t_index>& a_indices, const std::vector<glm::vec3>& a_vertices, const std::vector<glm::vec2>& a_uvs = {}, const std::vector<glm::vec3>& a_normals = {});

		GLuint m_indexBuffer;
		GLuint m_vaos[VAOs::Count];
		GLuint m_vbos[VBOs::Count];

		size_t m_indexCount;
		size_t m_vertexCount;
	};

	std::vector<MeshEntry> m_entries;
};
