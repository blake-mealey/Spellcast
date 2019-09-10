#pragma once

#include "Component.h"
#include "Entity.h"

#include <json/json.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>

class Mesh;
class Texture;
class CubeMap;
class Material;
class Material;
class ShaderProgram;

class InjectedContentManager {
public:
    ~InjectedContentManager();

    nlohmann::json GetJsonData(const std::string& a_filePath, bool a_overwrite = false);
	
	Mesh* GetMesh(const std::string& a_filePath, bool a_overwrite = false);
	Texture* GetTexture(const std::string& a_filePath, bool a_overwrite = false);
	CubeMap* GetCubeMap(const std::string& a_filePath, bool a_overwrite = false);
	Material* GetMaterial(const std::string& a_filePath, bool a_overwrite = false);
	Material* GetMaterial(nlohmann::json& a_data, bool a_overwrite = false);
	ShaderProgram* GetShaderProgram(const std::string& a_programName);

	// static std::vector<entity_id> LoadScene(const std::string& a_filePath, entity_id a_parent = INVALID_ENTITY);
	EntityDesc* GetEntityDesc(const std::string& a_filePath, bool a_overwrite = false);
	EntityDesc* GetEntityDesc(nlohmann::json& a_data, bool a_overwrite = false);
	ComponentDesc* GetComponentDesc(const std::string& a_filePath, bool a_overwrite = false);
	ComponentDesc* GetComponentDesc(nlohmann::json& a_data, bool a_overwrite = false);

	bool ReadFile(const std::string& a_filePath, std::string& a_source);

	void MergeJson(nlohmann::json& a_obj0, nlohmann::json& a_obj1, bool a_overwrite = true);
	
	std::string GetContentPath(const std::string& a_filePath, const std::string& a_dirPath = "");
	void NoFileWarning(const char* a_fileType, const char* a_filePath);

	template <typename T>
	int EnumFromJson(nlohmann::json& a_data, const std::string& a_key, int a_default = 0);

	template <typename T>
	int EnumFromJson(nlohmann::json& a_data, int a_default = 0);

	template <typename T>
	T FromJson(nlohmann::json& a_data, const std::string& a_key, const T& a_default = T());

	template <typename T>
	T FromJson(nlohmann::json& a_data, const T& a_default = T());

	template <typename V>
	V VecFromJson(nlohmann::json& a_data, const std::string& a_key, const V& a_default = V());

	template <typename V>
	V VecFromJson(nlohmann::json& a_data, const V& a_default = V());

	template <typename K>
	glm::vec4 ColorFromJson(nlohmann::json& a_data, const K& a_key, const glm::vec4& a_default = glm::vec4(1.f));

	glm::vec4 ColorFromJson(nlohmann::json& a_data, const glm::vec4& a_default = glm::vec4(1.f));

private:

    template <typename K, typename V>
    void DestroyMap(std::unordered_map<K,V*> a_map);

	std::unordered_map<std::string, nlohmann::json> m_jsonData;
	std::unordered_map<std::string, Mesh*> m_meshes;
	std::unordered_map<std::string, Texture*> m_textures;
	std::unordered_map<std::string, CubeMap*> m_cubeMaps;
	std::unordered_map<std::string, Material*> m_materials;
	std::unordered_map<std::string, ShaderProgram*> m_shaders;
	std::unordered_map<std::string, ComponentDesc*> m_componentDescs;
	std::unordered_map<std::string, EntityDesc*> m_entityDescs;
};
