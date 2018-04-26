#include "PointLight.h"
#include "ContentManager.h"

using namespace nlohmann;
using namespace glm;

PointLightDesc::PointLightDesc(): m_power(0) {}

PointLightDesc::PointLightDesc(json& a_data) {
	m_color = ContentManager::ColorFromJson(a_data, "Color", ContentManager::COLOR_WHITE);
	m_power = ContentManager::FromJson(a_data, "Power", 10.f);
	m_transform = Transform(a_data);
}

void PointLightDesc::Create(Entity* a_entity) {
	auto light = World::CreateAndGetComponent<PointLight>();
	light->Init(this);
	light->GetTransform().SetParent(&a_entity->GetTransform());
	a_entity->AddComponent(light);
}




PointLight::PointLight() : m_power(0) {}

component_type PointLight::GetType() {
	return Component::GetType() | (1 << GetTypeIndex());
}

component_index PointLight::GetTypeIndex() {
	return ComponentTypeIndex::POINT_LIGHT;
}

bool PointLight::Init(const PointLightDesc* a_desc) {
	m_color = a_desc->m_color;
	m_power = a_desc->m_power;

	m_transform = a_desc->m_transform;

	return true;
}

const vec3& PointLight::GetColor() const {
	return m_color;
}

float PointLight::GetPower() const {
	return m_power;
}

const Transform& PointLight::GetTransform() const {
	return m_transform;
}

Transform& PointLight::GetTransform() {
	return m_transform;
}

vec3 PointLight::GetGlobalPosition() const {
	return m_transform.GetGlobalPosition();
}
