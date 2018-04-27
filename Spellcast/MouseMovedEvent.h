#pragma once

#include <glm/glm.hpp>

struct MouseMovedEvent {
	glm::vec2 m_position;
	glm::vec2 m_delta;
	glm::vec2 m_fromCenter;
};
