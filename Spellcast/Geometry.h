#pragma once

#include <glm/glm.hpp>

#define G_E        2.71828182845904523536   // e
#define G_LOG2_E   1.44269504088896340736   // log2(e)
#define G_LOG10_E  0.434294481903251827651  // log10(e)
#define G_LN2      0.693147180559945309417  // ln(2)
#define G_LN10     2.30258509299404568402   // ln(10)
#define G_PI       3.14159265358979323846   // pi
#define G_2_PI     6.28318530717958647692   // 2pi
#define G_PI_2     1.57079632679489661923   // pi/2
#define G_PI_4     0.785398163397448309616  // pi/4
#define G_SQRT2    1.41421356237309504880   // sqrt(2)

class Geometry {
public:
	static const glm::vec3 FORWARD;
	static const glm::vec3 RIGHT;
	static const glm::vec3 UP;

	static glm::vec3 Project(const glm::vec3& a_vectorA, const glm::vec3& a_vectorB);
	static glm::vec3 ProjectVectorOnPlane(const glm::vec3& a_vector, const glm::vec3& a_normal);
	static glm::vec3 ProjectPointOnPlane(const glm::vec3& a_point, const glm::vec3& a_planePoint, const glm::vec3& a_normal);
};
