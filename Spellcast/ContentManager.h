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

class ContentManager {
public:
    static const glm::vec4 COLOR_WHITE;
    static const glm::vec4 COLOR_LIGHT_GREY;
    static const glm::vec4 COLOR_BLACK;
    static const glm::vec4 COLOR_LIGHT_RED;
    static const glm::vec4 COLOR_RED;
    static const glm::vec4 COLOR_LIGHT_GREEN;
    static const glm::vec4 COLOR_GREEN;
    static const glm::vec4 COLOR_DARK_BLUE;
    static const glm::vec4 COLOR_LIGHT_BLUE;
    static const glm::vec4 COLOR_BLUE;
    static const glm::vec4 COLOR_YELLOW;
    static const glm::vec4 COLOR_CYAN;

	static nlohmann::json GetJsonData(const std::string& a_filePath, bool a_overwrite = false);
	
	static Mesh* GetMesh(const std::string& a_filePath, bool a_overwrite = false);
	static Texture* GetTexture(const std::string& a_filePath, bool a_overwrite = false);
	static CubeMap* GetCubeMap(const std::string& a_filePath, bool a_overwrite = false);
	static Material* GetMaterial(const std::string& a_filePath, bool a_overwrite = false);
	static Material* GetMaterial(nlohmann::json& a_data, bool a_overwrite = false);
	static ShaderProgram* GetShaderProgram(const std::string& a_programName);

	// static std::vector<entity_id> LoadScene(const std::string& a_filePath, entity_id a_parent = INVALID_ENTITY);
	static EntityDesc* GetEntityDesc(const std::string& a_filePath, bool a_overwrite = false);
	static EntityDesc* GetEntityDesc(nlohmann::json& a_data, bool a_overwrite = false);
	static ComponentDesc* GetComponentDesc(const std::string& a_filePath, bool a_overwrite = false);
	static ComponentDesc* GetComponentDesc(nlohmann::json& a_data, bool a_overwrite = false);

	static bool ReadFile(const std::string& a_filePath, std::string& a_source);

	static void MergeJson(nlohmann::json& a_obj0, nlohmann::json& a_obj1, bool a_overwrite = true);
	
	static std::string GetContentPath(const std::string& a_filePath, const std::string& a_dirPath = "");
	static void NoFileWarning(const char* a_fileType, const char* a_filePath);

	template <typename T>
	static T FromJson(nlohmann::json& a_data, const std::string& a_key, const T& a_default = T());

	template <typename T>
	static T FromJson(nlohmann::json& a_data, const T& a_default = T());

	template <typename V>
	static V VecFromJson(nlohmann::json& a_data, const std::string& a_key, const V& a_default = V());

	template <typename V>
	static V VecFromJson(nlohmann::json& a_data, const V& a_default = V());

	template <typename K>
	static glm::vec4 ColorFromJson(nlohmann::json& a_data, const K& a_key, const glm::vec4& a_default = glm::vec4(1.f));

	static glm::vec4 ColorFromJson(nlohmann::json& a_data, const glm::vec4& a_default = glm::vec4(1.f));
private:

	static std::unordered_map<std::string, nlohmann::json> s_jsonData;
	static std::unordered_map<std::string, Mesh*> s_meshes;
	static std::unordered_map<std::string, Texture*> s_textures;
	static std::unordered_map<std::string, CubeMap*> s_cubeMaps;
	static std::unordered_map<std::string, Material*> s_materials;
	static std::unordered_map<std::string, ShaderProgram*> s_shaders;
	static std::unordered_map<std::string, ComponentDesc*> s_componentDescs;
	static std::unordered_map<std::string, EntityDesc*> s_entityDescs;
};

template <typename T>
T ContentManager::FromJson(nlohmann::json& a_data, const std::string& a_key, const T& a_default) {
	return FromJson(a_data[a_key], a_default);
}

template <typename T>
T ContentManager::FromJson(nlohmann::json& a_data, const T& a_default) {
	if (a_data.is_null()) return a_default;
	return a_data.get<T>();
}

template <typename V>
V ContentManager::VecFromJson(nlohmann::json& a_data, const std::string& a_key, const V& a_default) {
	return VecFromJson(a_data[a_key], a_default);
}

template <typename V>
V ContentManager::VecFromJson(nlohmann::json& a_data, const V& a_default) {
	if (!a_data.is_array() || a_data.size() != sizeof(V) / sizeof(float)) return a_default;

	V vector;
	float* vectorData = glm::value_ptr(vector);
	for (float f : a_data) vectorData++[0] = f;

	return vector;
}

template <typename K>
glm::vec4 ContentManager::ColorFromJson(nlohmann::json& a_data, const K& a_key, const glm::vec4& a_default) {
	return ColorFromJson(a_data[a_key], a_default);
}
