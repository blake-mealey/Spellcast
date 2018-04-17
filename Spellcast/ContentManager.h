#pragma once

#include "Mesh.h"

#include <json/json.hpp>

#include <unordered_map>

class ContentManager {
public:
	static nlohmann::json& GetJsonData(const std::string& a_filePath, bool a_overwrite = false);
	
	static MeshPtr& GetMesh(const std::string& a_filePath, bool a_overwrite = false);

	static bool ReadFile(const std::string& a_filePath, std::string& a_source);
	
	static std::string GetContentPath(const std::string& a_filePath, const std::string& a_dirPath = "");
	static void NoFileWarning(const char* a_fileType, const char* a_filePath);
private:

	static std::unordered_map<std::string, nlohmann::json> s_jsonData;
	static std::unordered_map<std::string, MeshPtr> s_meshes;
};
