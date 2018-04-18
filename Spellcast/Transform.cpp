#include "Transform.h"
#include "Geometry.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "ContentManager.h"

using namespace glm;

Transform::Transform(const vec3& a_position, const vec3& a_scale, const quat& a_rotation, Transform* a_parent) : m_parent(a_parent) {
	SetPosition(a_position);
	SetScale(a_scale);
	SetRotation(a_rotation);
}

Transform::Transform(const nlohmann::json& a_data, Transform* a_parent) : m_parent(a_parent) {
	SetPosition(ContentManager::VecFromJson(a_data, "Position", vec3(0.f)));
	SetScale(ContentManager::VecFromJson(a_data, "Scale", vec3(1.f)));
	SetRotation(degrees(ContentManager::VecFromJson(a_data, "Rotation", vec3(0.f))));
}

Transform* Transform::GetParent() const {
	return m_parent;
}

const vec3& Transform::GetLocalPosition() const {
	return m_position;
}

const vec3& Transform::GetLocalScale() const {
	return m_scale;
}

const quat& Transform::GetLocalRotation() const {
	return m_rotation;
}

vec3 Transform::GetGlobalPosition() const {
	return GetTransformationMatrix() * vec4(0.f, 0.f, 0.f, 1.f);
}

vec3 Transform::GetGlobalScale() const {
	vec3 globalScale = m_scale;
	Transform* transform = m_parent;
	while (transform) {
		globalScale = transform->GetLocalScale() * globalScale;
		transform = transform->GetParent();
	}
	return globalScale;
}

quat Transform::GetGlobalRotation() const {
	quat globalRotation = m_rotation;
	Transform* transform = m_parent;
	while (transform) {
		globalRotation = transform->GetLocalRotation() * globalRotation;
		transform = transform->GetParent();
	}
	return globalRotation;
}

vec3 Transform::GetEulerAngles() const {
	return eulerAngles(m_rotation);
}

vec3 Transform::GetLocalDirection(const vec3& a_globalDirection) const {
	return inverse(GetTransformationMatrix()) * vec4(a_globalDirection, 0.f);
}

vec3 Transform::GetGlobalDirection(const vec3& a_localDirection) const {
	return GetTransformationMatrix() * vec4(a_localDirection, 0.f);
}

vec3 Transform::Forward() const {
	return GetGlobalDirection(Geometry::FORWARD);
}

vec3 Transform::Right() const {
	return GetGlobalDirection(Geometry::RIGHT);
}

vec3 Transform::Up() const {
	return GetGlobalDirection(Geometry::UP);
}

void Transform::SetPosition(const vec3& a_position) {
	m_position = a_position;
	m_translationMatrix = translate(mat4(), m_position);
	UpdateTransformationMatrix();
}

void Transform::SetScale(const vec3& a_scale) {
	m_scale = a_scale;
	m_scalingMatrix = scale(m_scale);
	UpdateTransformationMatrix();
}

void Transform::SetRotation(const quat& a_rotation) {
	m_rotation = a_rotation;
	m_rotationMatrix = toMat4(m_rotation);
	UpdateTransformationMatrix();
}

void Transform::SetRotation(const vec3& a_eulerAngles) {
	SetRotation(quat(a_eulerAngles));
}

void Transform::SetRotation(const vec3& a_axis, const float& a_radians) {
	SetRotation(angleAxis(a_radians, a_axis));
}

void Transform::Translate(const vec3& a_offset) {
	SetPosition(m_position + a_offset);
}

void Transform::Scale(const float& a_scale) {
	SetScale(m_scale * a_scale);
}

void Transform::Scale(const vec3& a_scale) {
	SetScale(m_scale * a_scale);
}

void Transform::Rotate(const quat& a_rotation) {
	SetRotation(a_rotation * m_rotation);
}

void Transform::Rotate(const vec3& a_eulerAngles) {
	Rotate(quat(a_eulerAngles));
}

void Transform::Rotate(const vec3& a_axis, const float& a_radians) {
	SetRotation(rotate(m_rotation, a_radians, a_axis));
}

void Transform::LookAt(const vec3& a_localPosition) {
	LookInDirection(a_localPosition - GetLocalPosition());
}

void Transform::LookInDirection(const vec3& a_localDirection) {
	const vec3 direction = normalize(a_localDirection);
	const vec3 rotAxis = cross(Geometry::FORWARD, direction);
	const float dot = glm::dot(Geometry::FORWARD, direction);
	const quat rot = quat(dot + 1.f, rotAxis);
	SetRotation(normalize(rot));
}

const mat4& Transform::GetTranslationMatrix() const {
	return m_translationMatrix;
}

const mat4& Transform::GetScalingMatrix() const {
	return m_scalingMatrix;
}

const mat4& Transform::GetRotationMatrix() const {
	return m_rotationMatrix;
}

const mat4& Transform::GetLocalTransformationMatrix() const {
	return m_transformationMatrix;
}

// TODO: Better caching (set flag and reset every frame)
mat4 Transform::GetTransformationMatrix() const {
	mat4 transformationMatrix = m_transformationMatrix;
	Transform* transform = GetParent();
	while (transform) {
		transformationMatrix = transform->GetLocalTransformationMatrix() * transformationMatrix;
		transform = transform->GetParent();
	}
	return transformationMatrix;
}

void Transform::UpdateTransformationMatrix() {
	m_transformationMatrix = m_translationMatrix * m_rotationMatrix * m_scalingMatrix;
}
