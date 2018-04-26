#include "SpotLight.h"
#include "ContentManager.h"
#include <glm/gtc/matrix_transform.inl>
#include "Geometry.h"

using namespace nlohmann;
using namespace glm;

SpotLightDesc::SpotLightDesc(): m_power(0), m_angleDegrees(0), m_shadowIntensity(0), m_castsShadows(false),
                                m_shadowMapSize(0) {}

SpotLightDesc::SpotLightDesc(json& a_data) {
	m_color = ContentManager::ColorFromJson(a_data, "Color", ContentManager::COLOR_WHITE);
	m_power = ContentManager::FromJson(a_data, "Power", 10.f);
	m_transform = Transform(a_data);
	m_direction = ContentManager::VecFromJson(a_data, "Direction", vec3(0.f, 0.f, 1.f));
	m_angleDegrees = ContentManager::FromJson(a_data, "Angle", 5.f);

	m_shadowIntensity = ContentManager::FromJson(a_data, "ShadowIntensity", 0.75f);
	m_castsShadows = ContentManager::FromJson(a_data, "CastsShadows", true);
	m_shadowMapSize = ContentManager::FromJson(a_data, "ShadowMapSize", DEFAULT_SHADOW_MAP_SIZE);
}

void SpotLightDesc::Create(Entity* a_entity) {
	auto light = World::CreateAndGetComponent<SpotLight>();
	light->Init(this);
	light->GetTransform().SetParent(&a_entity->GetTransform());
	a_entity->AddComponent(light);
}




SpotLight::SpotLight() : m_power(0), m_angleDegrees(0) {}

component_type SpotLight::GetType() {
	return Component::GetType() | (1 << GetTypeIndex());
}

component_index SpotLight::GetTypeIndex() {
	return ComponentTypeIndex::SPOT_LIGHT;
}

bool SpotLight::Init(const SpotLightDesc* a_desc) {
	m_color = a_desc->m_color;
	m_power = a_desc->m_power;
	m_transform = a_desc->m_transform;
	m_direction = a_desc->m_direction;
	m_angleDegrees = a_desc->m_angleDegrees;

	m_shadowIntensity = a_desc->m_shadowIntensity;
	m_castsShadows = a_desc->m_castsShadows;
	m_shadowMapSize = a_desc->m_shadowMapSize;

	if (m_castsShadows) return InitShadowMap();

	return true;
}

void SpotLight::RenderShadowMap() {
	if (!m_enabled) return;
	ShadowCaster::RenderShadowMap();
}

void SpotLight::RenderShadows(const mat4& a_viewProjectionMatrix) const {
	if (!m_enabled) return;
	ShadowCaster::RenderShadows(a_viewProjectionMatrix);
}

void SpotLight::ComputeDepthViewProjectionMatrix() {
	const vec3 position = GetGlobalPosition();
	const mat4 viewMatrix = lookAt(position, position + GetGlobalDirection(), Geometry::UP);
	const mat4 projectionMatrix = perspective(GetAngleRadians()*2.f, 1.f, 0.5f, 50.f);
	m_depthViewProjectionMatrix = projectionMatrix * viewMatrix;
}

const vec3& SpotLight::GetColor() const {
	return m_color;
}

float SpotLight::GetPower() const {
	return m_power;
}

const Transform& SpotLight::GetTransform() const {
	return m_transform;
}

Transform& SpotLight::GetTransform() {
	return m_transform;
}

vec3 SpotLight::GetGlobalPosition() const {
	return m_transform.GetGlobalPosition();
}

const vec3& SpotLight::GetDirection() const {
	return m_direction;
}

vec3 SpotLight::GetGlobalDirection() const {
	return m_transform.GetGlobalDirection(m_direction);
}

float SpotLight::GetAngleDegrees() const {
	return m_angleDegrees;
}

float SpotLight::GetAngleRadians() const {
	return radians(m_angleDegrees);
}
