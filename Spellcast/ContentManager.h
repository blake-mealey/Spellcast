#pragma once

#include "Mesh.h"

#include <json/json.hpp>

#include <unordered_map>

class ContentManager {
public:
	static nlohmann::json& GetJsonData(const std::string& a_filePath, bool a_overwrite = false);
	
	static MeshPtr& GetMesh(const std::string& a_filePath, bool a_overwrite = false);

private:
	static std::string GetContentPath(const std::string& a_filePath);
	static void NoFileWarning(const char* a_fileType, const char* a_filePath);

	static std::unordered_map<std::string, nlohmann::json> m_jsonData;
	static std::unordered_map<std::string, MeshPtr> m_meshes;
};
