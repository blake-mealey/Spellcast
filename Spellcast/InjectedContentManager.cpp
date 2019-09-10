#include "InjectedContentManager.h"
#include "ContentManager.h"
#include "Mesh.h"

#include <fstream>
#include "CubeMap.h"
#include "LightingShader.h"
#include "CubeTerrain.h"
#include "FpsController.h"
#include "Logger.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionLight.h"
#include "MeshRenderer.h"
#include "SkyboxRenderer.h"
#include "Camera.h"

using namespace std;
using namespace glm;
using namespace nlohmann;

InjectedContentManager::~InjectedContentManager() {
    DestroyMap(m_meshes);
    DestroyMap(m_textures);
	DestroyMap(m_cubeMaps);
	DestroyMap(m_materials);
	DestroyMap(m_shaders);
	DestroyMap(m_componentDescs);
	DestroyMap(m_entityDescs);
}

template <typename K, typename V>
void InjectedContentManager::DestroyMap(std::unordered_map<K,V*> a_map) {
    for (auto iter = a_map.begin(); iter != a_map.end(); ++iter) {
        delete iter->second;
    }
}

json InjectedContentManager::GetJsonData(const std::string& a_filePath, bool a_overwrite) {
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
    ifstream file(a_filePath);
	if (file) {
		file >> data;
	} else {
		NoFileWarning("JSON", a_filePath.c_str());
	}
	file.close();

	// Cache the result and return
	m_jsonData[a_filePath] = data;
	return data;
}

Mesh* InjectedContentManager::GetMesh(const std::string& a_filePath, bool a_overwrite) {
    // Check if the data has already been loaded
	if (!a_overwrite) {
		const auto iter = m_meshes.find(a_filePath);
		if (iter != m_meshes.end()) {
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
	m_meshes[a_filePath] = mesh;
	return mesh;
}

Texture* InjectedContentManager::GetTexture(const std::string& a_filePath, bool a_overwrite) {
    // Check if the data has already been loaded
	if (!a_overwrite) {
		const auto iter = m_textures.find(a_filePath);
		if (iter != m_textures.end()) {
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
	m_textures[a_filePath] = texture;
	return texture;
}

CubeMap* InjectedContentManager::GetCubeMap(const std::string& a_filePath, bool a_overwrite) {
    // Check if the data has already been loaded
	if (!a_overwrite) {
		const auto iter = m_cubeMaps.find(a_filePath);
		if (iter != m_cubeMaps.end()) {
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
	m_cubeMaps[a_filePath] = cubeMap;
	return cubeMap;
}

Material* InjectedContentManager::GetMaterial(const std::string& a_filePath, bool a_overwrite) {
    string filePath = a_filePath;
	json data = filePath;
	return GetMaterial(data, a_overwrite);
}

Material* InjectedContentManager::GetMaterial(nlohmann::json& a_data, bool a_overwrite) {
    json data;
	const bool fromFile = a_data.is_string();
	if (fromFile) {
		const string filePath = FromJson<string>(a_data);
		
		// Check if the data has already been loaded
		if (!a_overwrite) {
			const auto iter = m_materials.find(filePath);
			if (iter != m_materials.end()) {
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
	if (fromFile) m_materials[FromJson<string>(a_data)] = mat;
	return mat;
}

ShaderProgram* InjectedContentManager::GetShaderProgram(const std::string& a_programName) {
    // Check if a shader with this name has already been initialized
	const auto iter = m_shaders.find(a_programName);
	if (iter != m_shaders.end()) {
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
	m_shaders[a_programName] = shader;
	return shader;
}

EntityDesc* InjectedContentManager::GetEntityDesc(const std::string& a_filePath, bool a_overwrite) {
    string filePath = a_filePath;
	json data = filePath;
	return GetEntityDesc(data, a_overwrite);
}

EntityDesc* InjectedContentManager::GetEntityDesc(nlohmann::json& a_data, bool a_overwrite) {
    // If we are loading from a file, check if we have already loaded the data
	const bool fromFile = a_data.is_string();
	EntityDesc* desc = nullptr;
	string filePath;
	if (fromFile) {
		filePath = a_data.get<string>();
		if (!a_overwrite) {
			const auto it = m_entityDescs.find(filePath);
			if (it != m_entityDescs.end()) {
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
		if (fromFile) m_entityDescs[filePath] = desc;
	}

	// Return the entity description
	return desc;
}

ComponentDesc* InjectedContentManager::GetComponentDesc(const std::string& a_filePath, bool a_overwrite) {
    string filePath = a_filePath;
	json data = filePath;
	return GetComponentDesc(data, a_overwrite);
}

ComponentDesc* InjectedContentManager::GetComponentDesc(nlohmann::json& a_data, bool a_overwrite) {
    // If we are loading from a file, check if we have already loaded the data
	const bool fromFile = a_data.is_string();
	ComponentDesc* desc = nullptr;
	string filePath;
	if (fromFile) {
		filePath = a_data.get<string>();
		if (!a_overwrite) {
			const auto it = m_componentDescs.find(filePath);
			if (it != m_componentDescs.end()) {
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
		case ComponentTypeIndex::SPOT_LIGHT:
			desc = new SpotLightDesc(a_data);
			break;
		case ComponentTypeIndex::POINT_LIGHT:
			desc = new PointLightDesc(a_data);
			break;
		case ComponentTypeIndex::FPS_CONTROLLER:
			desc = new FpsControllerDesc(a_data);
			break;
		case ComponentTypeIndex::CUBE_TERRAIN:
			desc = new CubeTerrainDesc(a_data);
			break;
		default: ;
		}

		// Cache the description
		if (fromFile) m_componentDescs[filePath] = desc;

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


bool InjectedContentManager::ReadFile(const std::string& a_filePath, std::string& a_source) {
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


void InjectedContentManager::MergeJson(nlohmann::json& a_obj0, nlohmann::json& a_obj1, bool a_overwrite) {
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


std::string InjectedContentManager::GetContentPath(const std::string& a_filePath, const std::string& a_dirPath) {
    return "../Content/" + a_dirPath + a_filePath;
}

void InjectedContentManager::NoFileWarning(const char* a_fileType, const char* a_filePath) {
    Logger::Console()->warn("Could not open {} file: {}", a_fileType, a_filePath);
}

vec4 InjectedContentManager::ColorFromJson(json& a_data, const vec4& a_default) {
	vec4 color = a_default;
    if (a_data.is_array()) {
        if (a_data.size() == 3) {
			color = vec4(VecFromJson<vec3>(a_data), 255.f) / 255.f;
        } else if (a_data.size() == 4) {
			color = VecFromJson<vec4>(a_data) / 255.f;
        }
    } else if (a_data.is_string()) {
        string name = a_data.get<string>();
        if (name.find("White") != string::npos)                 color = ContentManager::COLOR_WHITE;
        else if (name.find("LightGrey") != string::npos)        color = ContentManager::COLOR_LIGHT_GREY;
        else if (name.find("Black") != string::npos)            color = ContentManager::COLOR_BLACK;
        else if (name.find("LightRed") != string::npos)         color = ContentManager::COLOR_LIGHT_RED;
        else if (name.find("Red") != string::npos)              color = ContentManager::COLOR_RED;
        else if (name.find("LightGreen") != string::npos)       color = ContentManager::COLOR_LIGHT_GREEN;
        else if (name.find("Green") != string::npos)            color = ContentManager::COLOR_GREEN;
        else if (name.find("DarkBlue") != string::npos)         color = ContentManager::COLOR_DARK_BLUE;
        else if (name.find("LightBlue") != string::npos)        color = ContentManager::COLOR_LIGHT_BLUE;
        else if (name.find("Blue") != string::npos)             color = ContentManager::COLOR_BLUE;
        else if (name.find("Yellow") != string::npos)           color = ContentManager::COLOR_YELLOW;
        else if (name.find("Cyan") != string::npos)             color = ContentManager::COLOR_CYAN;

        if (name.find("HalfAlpha") != string::npos) color.a = 0.5f;
        else if (name.find("Alpha") != string::npos) color.a = 0.f;
    }
    return color;
}

template <typename T>
int InjectedContentManager::EnumFromJson(nlohmann::json& a_data, const std::string& a_key, const int a_default) {
	return EnumFromJson<T>(a_data[a_key], a_default);
}

template <typename T>
int InjectedContentManager::EnumFromJson(nlohmann::json& a_data, const int a_default) {
	if (a_data.is_string()) {
		const std::string name = a_data.get<std::string>();
		for (int i = 0; i < T::COUNT; ++i) if (T::NAMES[i] == name) return i;
	} else if (a_data.is_number_integer()) {
		const int value = a_data.get<int>();
		if (value >= 0 && value < T::COUNT) return value;
	}	
	return a_default;
}

template <typename T>
T InjectedContentManager::FromJson(nlohmann::json& a_data, const std::string& a_key, const T& a_default) {
	return FromJson(a_data[a_key], a_default);
}

template <typename T>
T InjectedContentManager::FromJson(nlohmann::json& a_data, const T& a_default) {
	if (a_data.is_null()) return a_default;
	return a_data.get<T>();
}

template <typename V>
V InjectedContentManager::VecFromJson(nlohmann::json& a_data, const std::string& a_key, const V& a_default) {
	return VecFromJson(a_data[a_key], a_default);
}

template <typename V>
V InjectedContentManager::VecFromJson(nlohmann::json& a_data, const V& a_default) {
	if (!a_data.is_array() || a_data.size() != sizeof(V) / sizeof(float)) return a_default;

	V vector;
	float* vectorData = glm::value_ptr(vector);
	for (float f : a_data) vectorData++[0] = f;

	return vector;
}

template <typename K>
glm::vec4 InjectedContentManager::ColorFromJson(nlohmann::json& a_data, const K& a_key, const glm::vec4& a_default) {
	return ColorFromJson(a_data[a_key], a_default);
}
