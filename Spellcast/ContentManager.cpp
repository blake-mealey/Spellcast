#include "ContentManager.h"
#include "LightingShader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "CubeMap.h"
#include "SkyboxRenderer.h"
#include "Logger.h"
#include "DirectionLight.h"

#include <fstream>

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
unordered_map<string, Mesh*> ContentManager::s_meshes;
unordered_map<string, Texture*> ContentManager::s_textures;
unordered_map<string, CubeMap*> ContentManager::s_cubeMaps;
unordered_map<string, Material*> ContentManager::s_materials;
unordered_map<string, ShaderProgram*> ContentManager::s_shaders;
unordered_map<string, ComponentDesc*> ContentManager::s_componentDescs;
unordered_map<string, EntityDesc*> ContentManager::s_entityDescs;

json ContentManager::GetJsonData(const string& a_filePath, const bool a_overwrite) {
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
	return data;
}

Mesh* ContentManager::GetMesh(const string& a_filePath, bool a_overwrite) {
	// Check if the data has already been loaded
	if (!a_overwrite) {
		const auto iter = s_meshes.find(a_filePath);
		if (iter != s_meshes.end()) {
			return iter->second;
		}
	}

	// Try loading from the file
	auto* mesh = new Mesh();
	if (!mesh->LoadFromFile(GetContentPath(a_filePath, "Meshes/"))) {
		delete mesh;
		mesh = nullptr;
	}

	// Cache the result and return
	s_meshes[a_filePath] = mesh;
	return mesh;
}

Texture* ContentManager::GetTexture(const std::string& a_filePath, bool a_overwrite) {
	// Check if the data has already been loaded
	if (!a_overwrite) {
		const auto iter = s_textures.find(a_filePath);
		if (iter != s_textures.end()) {
			return iter->second;
		}
	}

	// Try loading from the file
	auto* texture = new Texture();
	if (!texture->LoadFromFile(GetContentPath(a_filePath, "Textures/"))) {
		delete texture;
		texture = nullptr;
	}

	// Cache the result and return
	s_textures[a_filePath] = texture;
	return texture;
}

CubeMap* ContentManager::GetCubeMap(const std::string& a_filePath, bool a_overwrite) {
	// Check if the data has already been loaded
	if (!a_overwrite) {
		const auto iter = s_cubeMaps.find(a_filePath);
		if (iter != s_cubeMaps.end()) {
			return iter->second;
		}
	}

	// Try loading from the file
	auto* cubeMap = new CubeMap();
	if (!cubeMap->LoadFromFile(GetContentPath(a_filePath, "Textures/"))) {
		delete cubeMap;
		cubeMap = nullptr;
	}

	// Cache the result and return
	s_cubeMaps[a_filePath] = cubeMap;
	return cubeMap;
}

Material* ContentManager::GetMaterial(const std::string& a_filePath, bool a_overwrite) {
	string filePath = a_filePath;
	json data = filePath;
	return GetMaterial(data, a_overwrite);
}

Material* ContentManager::GetMaterial(json& a_data, bool a_overwrite) {
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
	auto* mat = new Material();
	if (!mat->Init(data)) {
		delete mat;
		mat = nullptr;
	}

	// Cache the result and return
	if (fromFile) s_materials[FromJson<string>(a_data)] = mat;
	return mat;
}

ShaderProgram* ContentManager::GetShaderProgram(const string& a_programName) {
	// Check if a shader with this name has already been initialized
	const auto iter = s_shaders.find(a_programName);
	if (iter != s_shaders.end()) {
		return iter->second;
	}

	// Try creating from name
	ShaderProgram* shader = nullptr;
	if (a_programName == "Lighting") {
		shader = new LightingShader();
	}

	// Try initializing
	if (shader && !shader->Init()) {
		delete shader;
		shader = nullptr;
	}

	// Cache the result and return
	s_shaders[a_programName] = shader;
	return shader;
}

EntityDesc* ContentManager::GetEntityDesc(const string& a_filePath, bool a_overwrite) {
	string filePath = a_filePath;
	json data = filePath;
	return GetEntityDesc(data, a_overwrite);
}

EntityDesc* ContentManager::GetEntityDesc(json& a_data, bool a_overwrite) {
	// If we are loading from a file, check if we have already loaded the data
	const bool fromFile = a_data.is_string();
	EntityDesc* desc = nullptr;
	string filePath;
	if (fromFile) {
		filePath = a_data.get<string>();
		if (!a_overwrite) {
			const auto it = s_entityDescs.find(filePath);
			if (it != s_entityDescs.end()) {
				desc = it->second;
			}
		}
	}

	// If the data is not already loaded, load it
	if (!desc) {
		// While we are given file path strings, load the next file
		while (a_data.is_string()) {
			a_data = GetJsonData(GetContentPath(a_data.get<string>(), "Entities/"), a_overwrite);
		}

		// While there is a super-entity, load and merge it
		json super = a_data["Extends"];
		while (!super.is_null()) {
			json superData = GetJsonData(GetContentPath(super.get<string>(), "Entities/"), a_overwrite);
			super = json(superData["Extends"]);
			MergeJson(superData, a_data);
			a_data = superData;
		}

		// Construct the description from the data
		desc = new EntityDesc(a_data);

		// Cache the description
		if (fromFile) s_entityDescs[filePath] = desc;
	}

	// Return the entity description
	return desc;
}

ComponentDesc* ContentManager::GetComponentDesc(const std::string& a_filePath, bool a_overwrite) {
	string filePath = a_filePath;
	json data = filePath;
	return GetComponentDesc(data, a_overwrite);
}

ComponentDesc* ContentManager::GetComponentDesc(json& a_data, bool a_overwrite) {
	// If we are loading from a file, check if we have already loaded the data
	const bool fromFile = a_data.is_string();
	ComponentDesc* desc = nullptr;
	string filePath;
	if (fromFile) {
		filePath = a_data.get<string>();
		if (!a_overwrite) {
			const auto it = s_componentDescs.find(filePath);
			if (it != s_componentDescs.end()) {
				desc = it->second;
			}
		}
	}

	// If the data is not already loaded, load it
	if (!desc) {
		// While we are given file path strings, load the next file
		while (a_data.is_string()) {
			a_data = GetJsonData(GetContentPath(a_data.get<string>(), "Components/"), a_overwrite);
		}

		// While there is a super-component, load and merge it
		json super = a_data["Extends"];
		while (!super.is_null()) {
			json superData = GetJsonData(GetContentPath(super.get<string>(), "Components/"), a_overwrite);
			super = json(superData["Extends"]);
			MergeJson(superData, a_data);
			a_data = superData;
		}

		// Get the component type index for the loaded data
		json type = a_data["Type"];
		if (!type.is_string()) {
			Logger::Console()->warn("Missing Type field in component data{}", fromFile ? " in " + filePath : "");
			return nullptr;
		}
		const component_index index = ComponentType::GetIndex(type.get<string>());

		// Construct the component description from the loaded data
		switch (index) {
		case ComponentTypeIndex::MESH_RENDERER:
			desc = new MeshRendererDesc(a_data);
			break;
		case ComponentTypeIndex::SKYBOX_RENDERER:
			desc = new SkyboxRendererDesc(a_data);
			break;
		case ComponentTypeIndex::CAMERA:
			desc = new CameraDesc(a_data);
			break;
		case ComponentTypeIndex::DIRECTION_LIGHT:
			desc = new DirectionLightDesc(a_data);
			break;
		default: ;
		}

		// Cache the description
		if (fromFile) s_componentDescs[filePath] = desc;

		// If no description was loaded, print an error and return null
		if (!desc) {
			if (a_data["Type"].is_null()) {
				Logger::Console()->warn("Component type not specified{}.", fromFile ? " in " + filePath : "");
			} else {
				Logger::Console()->warn("Unknown component type: {}{}", a_data["Type"].get<string>(),
					fromFile ? " in " + filePath : "");
			}
			delete desc;
			return nullptr;
		}
	}

	// Return the component description
	return desc;
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

void ContentManager::MergeJson(json& a_obj0, json& a_obj1, bool a_overwrite) {
    if (a_obj0.is_object()) {
        for (auto it = a_obj1.begin(); it != a_obj1.end(); ++it) {
            if (a_obj0[it.key()].is_primitive()) {
                if (a_overwrite || a_obj0[it.key()].is_null()) {
                    a_obj0[it.key()] = it.value();
                }
            } else {
                MergeJson(a_obj0[it.key()], it.value(), a_overwrite);
            }
        }
    } else if (a_obj0.is_array()) {
        if (a_obj0.size() >= 2 && a_obj0.size() <= 4 && a_obj0[0].is_number()) {
            a_obj0 = a_obj1;
        } else {
            for (json &value : a_obj1) {
                a_obj0.push_back(value);
            }
        }
    } else {
        a_obj0 = a_overwrite ? a_obj1 : a_obj0;
    }
}

string ContentManager::GetContentPath(const string& a_filePath, const string& a_dirPath) {
	return "../Content/" + a_dirPath + a_filePath;
}

void ContentManager::NoFileWarning(const char* a_fileType, const char* a_filePath) {
	Logger::Console()->warn("Could not open {} file: {}", a_fileType, a_filePath);
}

vec4 ContentManager::ColorFromJson(json& a_data, const vec4& a_default) {
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
