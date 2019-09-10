#include "JsonReader.h"

using namespace std;
using namespace glm;

const vec4 COLOR_WHITE =			vec4(1.f, 1.f, 1.f, 1.f);
const vec4 COLOR_LIGHT_GREY =		vec4(224.f, 224.f, 224.f, 255.f) / 255.f;
const vec4 COLOR_BLACK =			vec4(0.f, 0.f, 0.f, 1.f);
const vec4 COLOR_LIGHT_RED =		vec4(212.f, 47.f, 47.f, 255.f) / 255.f;
const vec4 COLOR_RED =				vec4(206.f, 0.f, 0.f, 255.f) / 255.f;
const vec4 COLOR_LIGHT_GREEN =		vec4(115.f, 212.f, 83.f, 255.f) / 255.f;
const vec4 COLOR_GREEN =			vec4(0.f, 1.f, 0.f, 1.f);
const vec4 COLOR_DARK_BLUE =		vec4(0.f, 57.f, 75.f, 255.f) / 255.f;
const vec4 COLOR_LIGHT_BLUE =		vec4(0.f, 106.f, 139.f, 255.f) / 255.f;
const vec4 COLOR_BLUE =				vec4(0.f, 0.f, 1.f, 1.f);
const vec4 COLOR_YELLOW =			vec4(255.f, 233.f, 25.f, 255.f) / 255.f;
const vec4 COLOR_CYAN =				vec4(0.f, 1.f, 1.f, 1.f);

JsonReader::JsonReader(nlohmann::json& a_data) {
    m_data = a_data;
}

vec4 JsonReader::GetColor(const vec4& a_default) {
	vec4 color = a_default;
    if (m_data.is_array()) {
        if (m_data.size() == 3) {
			color = vec4(GetVector<vec3>(), 255.f) / 255.f;
        } else if (m_data.size() == 4) {
			color = GetVector<vec4>() / 255.f;
        }
    } else if (m_data.is_string()) {
        string name = m_data.get<string>();
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