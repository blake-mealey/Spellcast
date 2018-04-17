#pragma once

#include <json/json.hpp>

#include <unordered_map>

class ContentManager {
public:
	static nlohmann::json& GetJsonData(std::string a_filePath, bool a_overwrite = false);

private:
	static std::string GetContentPath(std::string& a_filePath);
	static void NoFileWarning(const char* a_fileType, const char* a_filePath);

	static std::unordered_map<std::string, nlohmann::json> m_jsonData;
};