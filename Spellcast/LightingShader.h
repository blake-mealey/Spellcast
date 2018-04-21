#pragma once

#include "ShaderProgram.h"

#include <glm/glm.hpp>

class LightingShader : public ShaderProgram {
public:
	~LightingShader() override;
	bool Init() override;

	void SetMaterial(const Material* a_material) const override;
	void SetModelAndViewAndProjectionMatrices(const glm::mat4& a_modelMatrix, const glm::mat4& a_viewMatrix, const glm::mat4& a_projectionMatrix) const override;

	void SetModelMatrix(const glm::mat4& a_value) const;
	void SetViewMatrix(const glm::mat4& a_value) const;
	void SetModelViewProjectionMatrix(const glm::mat4& a_value) const;
	void SetDepthBiasModelViewProjectionMatrix(const glm::mat4& a_value) const;
	
	void SetMaterialColor(const glm::vec4& a_value) const;
	void SetMaterialSpecularColor(const glm::vec4& a_value) const;
	void SetMaterialSpecularity(const float& a_value) const;
	void SetMaterialEmission(const float& a_value) const;
	
	void SetAmbientColor(const glm::vec4& a_value) const;
	
	void SetShadowMapTextureUnit(const GLuint& a_value) const;
	void SetShadowsEnabled(const bool& a_value) const;
	
	void SetDiffuseTextureUnit(const GLuint& a_value) const;
	void SetDiffuseTextureEnabled(const bool& a_value) const;
	
	void SetUvScale(const glm::vec2& a_value) const;
	
	void SetBloomScale(const float& a_value) const;

	struct DirectionLight {
		DirectionLight(glm::vec3 a_color, glm::vec3 a_direction) :
			m_color(a_color), m_direction(a_direction) {}

		glm::vec3 m_color;				float __padding0[1];
		glm::vec3 m_direction;			float __padding1[1];
	};

	struct SpotLight {
		SpotLight(glm::vec3 a_color, float a_power, glm::vec3 a_position, float a_angle, glm::vec3 a_direction) :
			m_color(a_color), m_power(a_power), m_position(a_position), m_angle(a_angle), m_direction(a_direction) {}

		glm::vec3 m_color;
		float m_power;
		glm::vec3 m_position;
		float m_angle;
		glm::vec3 m_direction;			float __padding0[1];
	};

	struct PointLight {
		PointLight(glm::vec3 a_color, float a_power, glm::vec3 a_position) :
			m_color(a_color), m_power(a_power), m_position(a_position) {}

		glm::vec3 m_color;
		float m_power;
		glm::vec3 m_position;			float __padding0[1];
	};

	// TODO: Replace lights with single struct?
	/*struct Light {
		float m_type;
		glm::vec3 m_color;
		float m_power;
		glm::vec3 m_position;
		float m_angle;
		glm::vec3 m_direction;
	};*/

	struct SSBOs { enum { DirectionLights = 0, SpotLights, PointLights, Count }; };

	// TODO: void LoadLights(vector<LightComponent>)

	void LoadLights(const std::vector<DirectionLight>& a_directionLights,
		const std::vector<SpotLight>& a_spotLights, const std::vector<PointLight>& a_pointLights) const;
private:

	GLuint m_modelMatrixLocation = 0;
	GLuint m_viewMatrixLocation = 0;
	GLuint m_modelViewProjectionMatrixLocation = 0;
	GLuint m_depthBiasModelViewProjectionMatrixLocation = 0;

	GLuint m_materialDiffuseColorLocation = 0;
	GLuint m_materialSpecularColorLocation = 0;
	GLuint m_materialSpecularityLocation = 0;
	GLuint m_materialEmissivenessLocation = 0;
	
	GLuint m_ambientColorLocation = 0;
	
	GLuint m_shadowMapLocation = 0;
	GLuint m_shadowsEnabledLocation = 0;
	
	GLuint m_diffuseTextureLocation = 0;
	GLuint m_diffuseTextureEnabledLocation = 0;
	
	GLuint m_uvScaleLocation = 0;
	
	GLuint m_bloomScaleLocation = 0;

	GLuint m_ssbos[SSBOs::Count] = {};
};
