#include "ContentManager.h"
#include "LightingShader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "ShaderProgram.h"

#include <fstream>
#include <iostream>

using namespace std;
using namespace glm;
using namespace nlohmann;

const vec4 ContentManager::COLOR_WHITE =			vec4(1.f, 1.f, 1.f, 1.f);
const vec4 ContentManager::COLOR_LIGHT_GREY =		vec4(224.f, 224.f, 224.f, 255.f) / 255.f;
const vec4 ContentManager::COLOR_BLACK =			vec4(0.f, 0.f, 0.f, 1.f);
const vec4 ContentManager::COLOR_LIGHT_RED =		vec4(212.f, 47.f, 47.f, 255.f) / 255.f;
const vec4 ContentManager::COLOR_RED =				vec4(206.f, 0.f, 0.f, 255.f) / 255.f;
const vec4 ContentManager::COLOR_LIGHT_GREEN =		vec4(115.f, 212.f, 83.f, 255.f) / 255.f;
const vec4 ContentManager::COLOR_GREEN =			vec4(0.f, 1.f, 0.f, 1.f);
const vec4 ContentManager::COLOR_DARK_BLUE =		vec4(0.f, 57.f, 75.f, 255.f) / 255.f;
const vec4 ContentManager::COLOR_LIGHT_BLUE =		vec4(0.f, 106.f, 139.f, 255.f) / 255.f;
const vec4 ContentManager::COLOR_BLUE =				vec4(0.f, 0.f, 1.f, 1.f);
const vec4 ContentManager::COLOR_YELLOW =			vec4(255.f, 233.f, 25.f, 255.f) / 255.f;
const vec4 ContentManager::COLOR_CYAN =				vec4(0.f, 1.f, 1.f, 1.f);

unordered_map<string, json> ContentManager::s_jsonData;
unordered_map<string, MeshPtr> ContentManager::s_meshes;
unordered_map<string, TexturePtr> ContentManager::s_textures;
unordered_map<string, MaterialPtr> ContentManager::s_materials;
unordered_map<string, ShaderProgramPtr> ContentManager::s_shaders;

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
	ifstream file(a_filePath);
	if (file) {
		file >> data;
	} else {
		NoFileWarning("JSON", a_filePath.c_str());
	}
	file.close();

	// Cache the result and return
	s_jsonData[a_filePath] = data;
	return s_jsonData[a_filePath];
}

MeshPtr& ContentManager::GetMesh(const string& a_filePath, bool a_overwrite) {
	// Check if the data has already been loaded
	if (!a_overwrite) {
		const auto iter = s_meshes.find(a_filePath);
		if (iter != s_meshes.end()) {
			return iter->second;
		}
	}

	// Try loading from the file
	MeshPtr mesh = Mesh::Create();
	if (!mesh->LoadFromFile(GetContentPath(a_filePath, "Meshes/"))) {
		mesh = nullptr;
	}

	// Cache the result and return
	s_meshes[a_filePath] = mesh;
	return s_meshes[a_filePath];
}

TexturePtr& ContentManager::GetTexture(const std::string& a_filePath, bool a_overwrite) {
	// Check if the data has already been loaded
	if (!a_overwrite) {
		const auto iter = s_textures.find(a_filePath);
		if (iter != s_textures.end()) {
			return iter->second;
		}
	}

	// Try loading from the file
	TexturePtr texture = Texture::Create();
	if (!texture->LoadFromFile(GetContentPath(a_filePath, "Textures/"))) {
		texture = nullptr;
	}

	// Cache the result and return
	s_textures[a_filePath] = texture;
	return s_textures[a_filePath];
}

MaterialPtr ContentManager::GetMaterial(const json& a_data, bool a_overwrite) {
	json data;
	const bool fromFile = a_data.is_string();
	if (fromFile) {
		const string filePath = FromJson<string>(a_data);
		
		// Check if the data has already been loaded
		if (!a_overwrite) {
			const auto iter = s_materials.find(filePath);
			if (iter != s_materials.end()) {
				return iter->second;
			}
		}
		
		data = GetJsonData(GetContentPath(filePath, "Materials/"));
	} else {
		data = a_data;
	}

	// Try loading from the file
	MaterialPtr mat = Material::Create();
	if (!mat->Init(data)) {
		mat = nullptr;
	}

	// Cache the result and return
	if (fromFile) s_materials[FromJson<string>(a_data)] = mat;
	return mat;
}

ShaderProgramPtr& ContentManager::GetShaderProgram(const string& a_programName) {
	// Check if a shader with this name has already been initialized
	const auto iter = s_shaders.find(a_programName);
	if (iter != s_shaders.end()) {
		return iter->second;
	}

	// Try creating from name
	ShaderProgramPtr shader = nullptr;
	if (a_programName == "Lighting") {
		shader = LightingShader::Create();
	}

	// Try initializing
	if (shader && !shader->Init()) {
		shader = nullptr;
	}

	// Cache the result and return
	s_shaders[a_programName] = shader;
	return s_shaders[a_programName];
}

bool ContentManager::ReadFile(const string& a_filePath, string& a_source) {
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

string ContentManager::GetContentPath(const string& a_filePath, const string& a_dirPath) {
	return "../Content/" + a_dirPath + a_filePath;
}

void ContentManager::NoFileWarning(const char* a_fileType, const char* a_filePath) {
	cerr << "WARNING: Could not open " << a_fileType << " file: " << a_filePath << endl;
}

vec4 ContentManager::ColorFromJson(const json& a_data, const vec4& a_default) {
	vec4 color = a_default;
    if (a_data.is_array()) {
        if (a_data.size() == 3) {
			color = vec4(VecFromJson<vec3>(a_data), 255.f) / 255.f;
        } else if (a_data.size() == 4) {
			color = VecFromJson<vec4>(a_data) / 255.f;
        }
    } else if (a_data.is_string()) {
        string name = a_data.get<string>();
        if (name.find("White") != string::npos)                 color = COLOR_WHITE;
        else if (name.find("LightGrey") != string::npos)        color = COLOR_LIGHT_GREY;
        else if (name.find("Black") != string::npos)            color = COLOR_BLACK;
        else if (name.find("LightRed") != string::npos)         color = COLOR_LIGHT_RED;
        else if (name.find("Red") != string::npos)              color = COLOR_RED;
        else if (name.find("LightGreen") != string::npos)       color = COLOR_LIGHT_GREEN;
        else if (name.find("Green") != string::npos)            color = COLOR_GREEN;
        else if (name.find("DarkBlue") != string::npos)         color = COLOR_DARK_BLUE;
        else if (name.find("LightBlue") != string::npos)        color = COLOR_LIGHT_BLUE;
        else if (name.find("Blue") != string::npos)             color = COLOR_BLUE;
        else if (name.find("Yellow") != string::npos)           color = COLOR_YELLOW;
        else if (name.find("Cyan") != string::npos)             color = COLOR_CYAN;

        if (name.find("HalfAlpha") != string::npos) color.a = 0.5f;
        else if (name.find("Alpha") != string::npos) color.a = 0.f;
    }
    return color;
}
