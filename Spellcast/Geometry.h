#pragma once

#include <glm/glm.hpp>

class Geometry {
public:
	static const glm::vec3 FORWARD;
	static const glm::vec3 RIGHT;
	static const glm::vec3 UP;

	static glm::vec3 Project(const glm::vec3& a_vectorA, const glm::vec3& a_vectorB);
	static glm::vec3 ProjectVectorOnPlane(const glm::vec3& a_vector, const glm::vec3& a_normal);
	static glm::vec3 ProjectPointOnPlane(const glm::vec3& a_point, const glm::vec3& a_planePoint, const glm::vec3& a_normal);
};
