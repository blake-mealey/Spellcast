#pragma once

#include <glm/glm.hpp>

struct MouseMovedEvent {
	glm::vec2 m_position;
	glm::vec2 m_delta;
	glm::vec2 m_fromCenter;

	bool m_leftDown;
	bool m_rightDown;
	bool m_middleDown;
};
