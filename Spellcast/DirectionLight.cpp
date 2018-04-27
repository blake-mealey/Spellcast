#include "DirectionLight.h"
#include "Entity.h"
#include "ContentManager.h"

#include <glm/gtc/matrix_transform.inl>

using namespace glm;

DirectionLightDesc::DirectionLightDesc() : m_shadowIntensity(0.75f), m_castsShadows(true),
                                           m_shadowMapSize(DEFAULT_SHADOW_MAP_SIZE) {}

DirectionLightDesc::DirectionLightDesc(nlohmann::json& a_data) {
	m_color = ContentManager::ColorFromJson(a_data, "Color", ContentManager::COLOR_WHITE);
	m_direction = ContentManager::VecFromJson(a_data, "Direction", vec3(0, 0, 1));

	m_shadowIntensity = ContentManager::FromJson(a_data, "ShadowIntensity", 0.75f);
	m_castsShadows = ContentManager::FromJson(a_data, "CastsShadows", true);
	m_shadowMapSize = ContentManager::FromJson(a_data, "ShadowMapSize", DEFAULT_SHADOW_MAP_SIZE);
}

void DirectionLightDesc::Create(Entity* a_entity) {
	auto* light = World::CreateAndGetComponent<DirectionLight>();
	light->Init(this);
	a_entity->AddComponent(light);
}




DirectionLight::~DirectionLight() = default;
DirectionLight::DirectionLight() = default;

component_type DirectionLight::GetType() {
	return Component::GetType() | (1 << GetTypeIndex());
}

component_index DirectionLight::GetTypeIndex() {
	return ComponentTypeIndex::DIRECTION_LIGHT;
}

bool DirectionLight::Init(const DirectionLightDesc* a_desc) {
	m_color = a_desc->m_color;
	m_direction = normalize(a_desc->m_direction);

	m_shadowIntensity = a_desc->m_shadowIntensity;
	m_castsShadows = a_desc->m_castsShadows;
	m_shadowMapSize = a_desc->m_shadowMapSize;

	if (m_castsShadows) return InitShadowMap();

	return true;
}

bool DirectionLight::InitShadowMap() {
	return ShadowCaster::InitShadowMap();
}

void DirectionLight::RenderShadowMap() {
	if (!m_enabled) return;
	ShadowCaster::RenderShadowMap();
}

void DirectionLight::RenderShadows(const mat4& a_viewProjectionMatrix) const {
	if (!m_enabled) return;
	ShadowCaster::RenderShadows(a_viewProjectionMatrix);
}

void DirectionLight::ComputeDepthViewProjectionMatrix() {
	const mat4 depthProjectionMatrix = ortho<float>(-150, 150, -75, 75, -200, 200);
	const mat4 depthViewMatrix = lookAt(-m_direction, vec3(0), vec3(0, 1, 0));
	m_depthViewProjectionMatrix = depthProjectionMatrix * depthViewMatrix;
}

const vec3& DirectionLight::GetColor() const {
	return m_color;
}

const vec3& DirectionLight::GetDirection() const {
	return m_direction;
}

void DirectionLight::SetDirection(const vec3& a_direction) {
	m_direction = normalize(a_direction);
}
