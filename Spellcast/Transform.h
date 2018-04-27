#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <json/json.hpp>

class Transform {
public:
	explicit Transform(const glm::vec3& a_position = glm::vec3(0.f),
		const glm::vec3& a_scale = glm::vec3(1.f),
		const glm::quat& a_rotation = glm::quat(),
		Transform* a_parent = nullptr);

	explicit Transform(nlohmann::json& a_data, Transform* a_parent = nullptr);

	Transform* GetParent() const;
	void SetParent(Transform* a_transform);

	const glm::vec3& GetLocalPosition() const;
	const glm::vec3& GetLocalScale() const;
	const glm::quat& GetLocalRotation() const;

	glm::vec3 GetGlobalPosition() const;
	glm::vec3 GetGlobalScale() const;
	glm::quat GetGlobalRotation() const;

	glm::vec3 GetEulerAngles() const;

	glm::vec3 GetLocalDirection(const glm::vec3& a_globalDirection) const;
	glm::vec3 GetGlobalDirection(const glm::vec3& a_localDirection) const;
	glm::vec3 Forward() const;
	glm::vec3 Right() const;
	glm::vec3 Up() const;

	void SetPosition(const glm::vec3& a_position);
	void SetScale(const glm::vec3& a_scale);
	void SetRotation(const glm::quat& a_rotation);
	void SetRotation(const glm::vec3& a_eulerAngles);
	void SetRotation(const glm::vec3& a_axis, const float& a_radians);

	void Translate(const glm::vec3& a_offset);
	void TranslateLocal(const glm::vec3& a_offset);
	void Scale(const float& a_scale);
	void Scale(const glm::vec3& a_scale);
	void Rotate(const glm::quat& a_rotation);
	void Rotate(const glm::vec3& a_eulerAngles);
	void Rotate(const glm::vec3& a_axis, const float& a_radians);

	void LookAt(const glm::vec3& a_localPosition);
	void LookInDirection(const glm::vec3& a_localDirection);

	const glm::mat4& GetTranslationMatrix() const;
	const glm::mat4& GetScalingMatrix() const;
	const glm::mat4& GetRotationMatrix() const;
	const glm::mat4& GetLocalTransformationMatrix() const;
	glm::mat4 GetTransformationMatrix() const;

private:
	void UpdateTransformationMatrix();

	Transform* m_parent;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::quat m_rotation;

	glm::mat4 m_translationMatrix;
	glm::mat4 m_scalingMatrix;
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_transformationMatrix;
};
