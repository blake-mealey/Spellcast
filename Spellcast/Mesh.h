#pragma once

#include "Material.h"

#include <vector>

class RenderContext;
struct aiScene;

typedef unsigned int t_index;

class Mesh {
public:
	Mesh();
	~Mesh();

	bool LoadFromFile(const std::string& a_filePath, unsigned a_flags = 0);

	void Render(const RenderContext* a_context) const;
	void RenderBasic() const;

	struct VAOs { enum { Geometry = 0, Count }; };
	struct VBOs { enum { Vertices = 0, Normals, Uvs, Count }; };

private:
	void Init(const aiScene* scene);

	typedef unsigned GLuint;
	
	struct MeshEntry {
		MeshEntry();
		~MeshEntry();

		bool Init(const std::vector<t_index>& a_indices, const std::vector<glm::vec3>& a_vertices, const std::vector<glm::vec2>& a_uvs = {}, const std::vector<glm::vec3>& a_normals = {});

		void Render(const RenderContext* a_context) const;
		void RenderBasic() const;

		GLuint m_indexBuffer;
		GLuint m_vaos[VAOs::Count];
		GLuint m_vbos[VBOs::Count];

		size_t m_indexCount;
		size_t m_vertexCount;
		
		size_t m_materialIndex;
	};

	std::vector<MeshEntry> m_entries;
};
