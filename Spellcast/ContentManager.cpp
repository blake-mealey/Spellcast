#include "ContentManager.h"

#include <fstream>
#include <iostream>

using namespace std;
using namespace nlohmann;

unordered_map<string, json> ContentManager::m_jsonData;

json& ContentManager::GetJsonData(string a_filePath, bool a_overwrite) {
	// Check if the data has already been loaded
	if (!a_overwrite) {
		const auto iter = m_jsonData.find(a_filePath);
		if (iter != m_jsonData.end()) {
			return iter->second;
		}
	}

	// Initialize the data to null
	json data;

	// Try to load from the file
	const std::string contentPath = GetContentPath(a_filePath);
	ifstream file(contentPath);
	if (file) {
		file >> data;
	} else {
		NoFileWarning("JSON", contentPath.c_str());
	}
	file.close();

	// Cache the result and return
	m_jsonData[a_filePath] = data;
	return m_jsonData[a_filePath];
}

std::string ContentManager::GetContentPath(std::string& a_filePath) {
	return "../Content/" + a_filePath;
}

void ContentManager::NoFileWarning(const char* a_fileType, const char* a_filePath) {
	cerr << "WARNING: Could not open " << a_fileType << " file: " << a_filePath << endl;
}
