#pragma once

#include <json/json.hpp>
#include <glm/gtc/type_ptr.hpp>

class JsonReader {
public:
    explicit JsonReader(nlohmann::json& a_data);

    template <typename T>
	int GetEnum(const std::string& a_key, int a_default = 0);

	template <typename T>
	int GetEnum(int a_default = 0);

	template <typename T>
	T GetValue(const std::string& a_key, const T& a_default = T());

	template <typename T>
	T GetValue(const T& a_default = T());

	template <typename V>
	V GetVector(const std::string& a_key, const V& a_default = V());

	template <typename V>
	V GetVector(const V& a_default = V());

	template <typename K>
	glm::vec4 GetColor(const K& a_key, const glm::vec4& a_default = glm::vec4(1.f));

	glm::vec4 GetColor(const glm::vec4& a_default = glm::vec4(1.f));

private:
    nlohmann::json m_data;
};

template <typename K>
glm::vec4 JsonReader::GetColor(const K& a_key, const glm::vec4& a_default) {
	return JsonReader(m_data[a_key]).GetColor(a_default);
}

template <typename T>
int JsonReader::GetEnum(const std::string& a_key, const int a_default) {
	return JsonReader(m_data[a_key]).GetEnum<T>(a_default);
}

template <typename T>
int JsonReader::GetEnum(const int a_default) {
	if (m_data.is_string()) {
		const std::string name = m_data.get<std::string>();
		for (int i = 0; i < T::COUNT; ++i) if (T::NAMES[i] == name) return i;
	} else if (m_data.is_number_integer()) {
		const int value = m_data.get<int>();
		if (value >= 0 && value < T::COUNT) return value;
	}	
	return a_default;
}

template <typename T>
T JsonReader::GetValue(const std::string& a_key, const T& a_default) {
	return JsonReader(m_data[a_key]).GetValue(a_default);
}

template <typename T>
T JsonReader::GetValue(const T& a_default) {
	if (m_data.is_null()) return a_default;
	return m_data.get<T>();
}

template <typename V>
V JsonReader::GetVector(const std::string& a_key, const V& a_default) {
	return JsonReader(m_data[a_key]).GetVector(a_default);
}

template <typename V>
V JsonReader::GetVector(const V& a_default) {
	if (!m_data.is_array() || m_data.size() != sizeof(V) / sizeof(float)) return a_default;

	V vector;
	float* vectorData = value_ptr(vector);
	for (float f : m_data) vectorData++[0] = f;

	return vector;
}
