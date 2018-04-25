#include "DirectionLight.h"
#include "Entity.h"
#include "ContentManager.h"
#include "Logger.h"
#include "MeshRenderer.h"
#include <glm/gtc/matrix_transform.inl>

using namespace glm;

#define DEFAULT_SHADOW_MAP_SIZE 1024

DirectionLightDesc::DirectionLightDesc() : m_castsShadows(true), m_shadowMapSize(DEFAULT_SHADOW_MAP_SIZE) {}

DirectionLightDesc::DirectionLightDesc(nlohmann::json& a_data) {
	m_color = ContentManager::ColorFromJson(a_data["Color"], ContentManager::COLOR_WHITE);
	m_direction = ContentManager::VecFromJson(a_data["Direction"], vec3(0, 0, 1));
	
	m_castsShadows = ContentManager::FromJson(a_data["CastsShadows"], true);
	m_shadowMapSize = ContentManager::FromJson(a_data["ShadowMapSize"], DEFAULT_SHADOW_MAP_SIZE);
}

void DirectionLightDesc::Create(Entity* a_entity) {
	auto* light = World::CreateAndGetComponent<DirectionLight>();
	light->Init(this);
	a_entity->AddComponent(light);
}




DirectionLight::~DirectionLight() {
	if (m_shadowBuffer) glDeleteFramebuffers(1, &m_shadowBuffer);
}

DirectionLight::DirectionLight() : m_shadowBuffer(0), m_castsShadows(false), m_shadowMapSize(0) {}

component_type DirectionLight::GetType() {
	return Component::GetType() | (1 << GetTypeIndex());
}

component_index DirectionLight::GetTypeIndex() {
	return ComponentTypeIndex::DIRECTION_LIGHT;
}

bool DirectionLight::Init(const DirectionLightDesc* a_desc) {
	m_color = a_desc->m_color;
	m_direction = a_desc->m_direction;

	m_castsShadows = a_desc->m_castsShadows;
	m_shadowMapSize = a_desc->m_shadowMapSize;

	if (m_castsShadows) return InitShadowMap();

	return true;
}

bool DirectionLight::InitShadowMap() {
	if (!m_shadowShader.Init()) return false;

	glGenFramebuffers(1, &m_shadowBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer);

	// Add depth texture
	m_shadowMap.Init(GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_FLOAT, m_shadowMapSize, m_shadowMapSize, nullptr, GL_CLAMP_TO_EDGE);
	m_shadowMap.SetParameter(GL_TEXTURE_COMPARE_FUNC, GL_GEQUAL);
	m_shadowMap.SetParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMap.GetId(), 0);

	// No draw buffers
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logger::Console()->warn("Shadow map framebuffer incomplete.");
	
	return glGetError() == GL_NO_ERROR;
}

void DirectionLight::RenderShadowMap() {
	if (!m_enabled || !m_castsShadows) return;

	// Render to the shadow map framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer);

	// Clear the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	m_shadowShader.Enable();

	glViewport(0, 0, m_shadowMapSize, m_shadowMapSize);

	// TODO: Determine the size of the ortho matrix correctly
	const mat4 depthProjectionMatrix = ortho<float>(-150, 150, -75, 75, -200, 200);
	const mat4 depthViewMatrix = lookAt(-m_direction, vec3(0), vec3(0, 1, 0));
	m_depthViewProjectionMatrix = depthProjectionMatrix * depthViewMatrix;

	for (auto it = World::BeginComponents<MeshRenderer>(); it != World::EndComponents<MeshRenderer>(); ++it) {
		const MeshRenderer& meshRenderer = *it;
		if (!meshRenderer.IsEnabled() || !meshRenderer.DoesCastShadows()) continue;
		
		const mat4& depthModelMatrix = meshRenderer.GetTransform().GetTransformationMatrix();
		m_shadowShader.SetDepthModelViewProjectionMatrix(m_depthViewProjectionMatrix * depthModelMatrix);
		meshRenderer.RenderBasic();
	}
}

void DirectionLight::SetCastsShadows(const bool a_castsShadows) {
	m_castsShadows = a_castsShadows;
	if (!m_shadowBuffer) InitShadowMap();
}

const Texture& DirectionLight::GetShadowMap() const {
	return m_shadowMap;
}

const mat4& DirectionLight::GetDepthViewProjectionMatrix() const {
	return m_depthViewProjectionMatrix;
}

const vec3& DirectionLight::GetColor() const {
	return m_color;
}

const vec3& DirectionLight::GetDirection() const {
	return m_direction;
}

bool DirectionLight::CastsShadows() const {
	return m_castsShadows;
}
