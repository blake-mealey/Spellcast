#include "Geometry.h"

using namespace glm;

const vec3 Geometry::FORWARD = vec3(0.f, 0.f, 1.f);
const vec3 Geometry::RIGHT = vec3(-1.f, 0.f, 0.f);
const vec3 Geometry::UP = vec3(0.f, 1.f, 0.f);

vec3 Geometry::Project(const vec3& a_vectorA, const vec3& a_vectorB) {
	return (dot(a_vectorA, a_vectorB) / dot(a_vectorB, a_vectorB)) * a_vectorB;
}

vec3 Geometry::ProjectVectorOnPlane(const vec3& a_vector, const vec3& a_normal) {
	return a_vector - Project(a_vector, a_normal);
}

vec3 Geometry::ProjectPointOnPlane(const vec3& a_point, const vec3& a_planePoint, const vec3& a_normal) {
	return a_point - Project(a_point - a_planePoint, a_normal);
}
