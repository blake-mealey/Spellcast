#pragma once

#include "ShaderProgram.h"

class LightingShader : public ShaderProgram {
public:
	~LightingShader() override;
	bool Init() override;

	void SetMaterial(const Material* a_material) const override;
	void SetModelAndViewAndProjectionMatrices(const glm::mat4& a_modelMatrix, const glm::mat4& a_viewMatrix, const glm::mat4& a_projectionMatrix) const override;

	void SetModelMatrix(const glm::mat4& a_value) const;
	void SetViewMatrix(const glm::mat4& a_value) const;
	void SetModelViewProjectionMatrix(const glm::mat4& a_value) const;
	
	void SetMaterialColor(const glm::vec4& a_value) const;
	void SetMaterialSpecularColor(const glm::vec4& a_value) const;
	void SetMaterialSpecularity(float a_value) const;
	void SetMaterialEmission(float a_value) const;
	
	void SetAmbientColor(const glm::vec4& a_value) const;
	
	void SetAlbedoMapUnit(GLuint a_value) const;
	void SetAlbedoMapEnabled(bool a_value) const;
	
	void SetUvScale(const glm::vec2& a_value) const;
	
	void SetGlowScale(float a_value) const;

	struct DirectionLightData {
		DirectionLightData(const glm::vec3& a_color, const glm::vec3& a_direction) :
			m_color(a_color), __padding0{}, m_direction(a_direction), __padding1{} {}

		glm::vec3 m_color;				float __padding0[1];
		glm::vec3 m_direction;			float __padding1[1];
	};

	struct SpotLightData {
		SpotLightData(const glm::vec3& a_color, const float a_power, const glm::vec3& a_position, const float a_angle, const glm::vec3& a_direction) :
			m_color(a_color), m_power(a_power), m_position(a_position), m_angle(a_angle), m_direction(a_direction),
			__padding0{} {}

		glm::vec3 m_color;
		float m_power;
		glm::vec3 m_position;
		float m_angle;
		glm::vec3 m_direction;			float __padding0[1];
	};

	struct PointLightData {
		PointLightData(const glm::vec3& a_color, const float a_power, const glm::vec3& a_position) :
			m_color(a_color), m_power(a_power), m_position(a_position), __padding0{} {}

		glm::vec3 m_color;
		float m_power;
		glm::vec3 m_position;			float __padding0[1];
	};

	struct SSBOs { enum { DirectionLights = 0, SpotLights, PointLights, Count }; };

	void LoadLights(const std::vector<DirectionLightData>& a_directionLights,
		const std::vector<SpotLightData>& a_spotLights, const std::vector<PointLightData>& a_pointLights) const;

	void LoadLights() const;

private:
	uniform_loc m_modelMatrixLoc = 0;
	uniform_loc m_viewMatrixLoc = 0;
	uniform_loc m_modelViewProjectionMatrixLoc = 0;

	uniform_loc m_materialAlbedoColorLoc = 0;
	uniform_loc m_materialSpecularColorLoc = 0;
	uniform_loc m_materialSpecularityLoc = 0;
	uniform_loc m_materialEmissionLoc = 0;
	
	uniform_loc m_ambientColorLoc = 0;
	
	uniform_loc m_albedoMapLoc = 0;
	uniform_loc m_albedoMapEnabledLoc = 0;
	
	uniform_loc m_uvScaleLoc = 0;
	
	uniform_loc m_glowScaleLoc = 0;

	GLuint m_ssbos[SSBOs::Count] = {};
};
