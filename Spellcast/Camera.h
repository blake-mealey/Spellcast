#pragma once

#include "Component.h"
#include "CopyShader.h"
#include "BlurShader.h"
#include "Texture.h"

#include <json/json.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Entity;

#define BLUR_LEVEL_COUNT 4

#define DEFAULT_NEAR_CLIPPING_PLANE 0.1f
#define DEFAULT_FAR_CLIPPING_PLANE 1000.f
#define DEFAULT_FIELD_OF_VIEW 60.f

class Graphics;

class Camera;

struct CameraMode {
	enum { TARGET = 0, SPHERE, FPS, COUNT };
	static const std::string NAMES[COUNT];
};

struct CameraDesc : ComponentDesc {
	CameraDesc();
	explicit CameraDesc(nlohmann::json& a_data);
	void Create(Entity* a_entity) override;

	float m_nearClippingPlane;
	float m_farClippingPlane;
	
	glm::vec3 m_upVector;
	float m_fieldOfView;

	glm::vec3 m_localPosition;
	glm::vec3 m_targetGlobalPosition;

	glm::vec2 m_viewportUnitScale;
	glm::vec2 m_viewportUnitPosition;
	
	glm::vec2 m_viewportPixelScale;
	glm::vec2 m_viewportPixelPosition;

	float m_horizontalAngle;
	float m_verticalAngle;

	int m_mode;
};

class Camera : public Component {
friend SlotMap<Camera>;
public:
	~Camera() override;
	Camera();
	static component_type GetType();
	static component_index GetTypeIndex();

	bool Init(const CameraDesc& a_desc);

	void Render(const Graphics& a_context);

	void SetLocalPosition(const glm::vec3& a_position);
	glm::vec3 GetGlobalPosition() const;
	
	glm::vec3 GetGlobalTarget() const;
	glm::vec3 GetGlobalForward() const;
	glm::vec3 GetGlobalRight() const;
	glm::vec3 GetGlobalUp() const;

	void SetViewportUnitScale(const glm::vec2& a_scale);
	void SetViewportUnitPosition(const glm::vec2& a_position);
	
	void TranslateAngles(const glm::vec2& a_offset);
	
	bool IsMode(int a_mode) const;

private:
	CopyShader m_copyShader;
	BlurShader m_blurShader;

	bool GenerateBuffers();
	bool InitVaoAndVbo();
	bool InitGlowBuffer();
	bool InitScreenBuffer();

	GLuint m_depthStencilBuffer;
	GLuint m_screenBuffer;
	GLuint m_glowBuffer;

	GLuint m_screenVao;
	GLuint m_screenVbo;

	Texture m_screenTexture;
	Texture m_glowTexture;

	Texture m_blurTextures[BLUR_LEVEL_COUNT];
	Texture m_blurTempTextures[BLUR_LEVEL_COUNT];

	float m_nearClippingPlane;
	float m_farClippingPlane;
	
	glm::vec3 m_upVector;
	float m_fieldOfView;

	glm::vec3 m_localPosition;
	glm::vec3 m_targetGlobalPosition;

	glm::vec2 m_viewportUnitScale;
	glm::vec2 m_viewportUnitPosition;
	
	glm::vec2 m_viewportPixelScale;
	glm::vec2 m_viewportPixelPosition;

	glm::vec2 m_viewportScale;
	glm::vec2 m_viewportPosition;

	float m_horizontalAngle;
	float m_verticalAngle;

	int m_mode;
};
