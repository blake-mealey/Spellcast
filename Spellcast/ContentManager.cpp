#include "ContentManager.h"

#include <fstream>
#include <iostream>

using namespace std;
using namespace nlohmann;

unordered_map<string, json> ContentManager::s_jsonData;
unordered_map<string, MeshPtr> ContentManager::s_meshes;

json& ContentManager::GetJsonData(const string& a_filePath, const bool a_overwrite) {
	// Check if the data has already been loaded
	if (!a_overwrite) {
		const auto iter = s_jsonData.find(a_filePath);
		if (iter != s_jsonData.end()) {
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
	s_jsonData[a_filePath] = data;
	return s_jsonData[a_filePath];
}

MeshPtr& ContentManager::GetMesh(const std::string& a_filePath, bool a_overwrite) {
	// Check if the data has already been loaded
	if (!a_overwrite) {
		const auto iter = s_meshes.find(a_filePath);
		if (iter != s_meshes.end()) {
			return iter->second;
		}
	}

	// Try loading from the file
	MeshPtr mesh = Mesh::Create();
	mesh->LoadFromFile(GetContentPath(a_filePath, "Meshes/"));

	// Cache the result and return
	s_meshes[a_filePath] = mesh;
	return s_meshes[a_filePath];
}

bool ContentManager::ReadFile(const std::string& a_filePath, std::string& a_source) {
	ifstream input(a_filePath.c_str());
	if (input) {
		copy(istreambuf_iterator<char>(input),
			istreambuf_iterator<char>(),
			back_inserter(a_source));
		input.close();
		return true;
	}
	
	return false;
}

std::string ContentManager::GetContentPath(const std::string& a_filePath, const std::string& a_dirPath) {
	return "../Content/" + a_dirPath + a_filePath;
}

void ContentManager::NoFileWarning(const char* a_fileType, const char* a_filePath) {
	cerr << "WARNING: Could not open " << a_fileType << " file: " << a_filePath << endl;
}