#include "Simulation.h"
#include "Logger.h"

#include <glm/gtx/string_cast.hpp>

using namespace glm;

Simulation::Simulation() = default;
Simulation::~Simulation() = default;

Simulation& Simulation::Instance() {
	static Simulation instance;
	return instance;
}

bool Simulation::Init() {
	return true;
}

void Simulation::Update(const Time& a_deltaTime, const Time& a_globalTime) {
	// Rotate entities... for reasons
	// for (auto it = World::BeginEntities(); it != World::EndEntities(); ++it) {
		// it->GetTransform().Rotate(Geometry::UP, 0.01f);
	// }

	// constexpr float distance = 10.f;
	// constexpr float speed = 0.3f;
	// for (auto it = World::BeginComponents<Camera>(); it != World::EndComponents<Camera>(); ++it) {
		// it->SetGlobalPosition(distance * vec3(
			// cos(a_globalTime.GetSeconds()*speed), 0.1f, sin(a_globalTime.GetSeconds()*speed)));
	// }
}

void Simulation::On(const KeyboardEvent& a_event) {
	Logger::Console()->info("Keyboard Event: {} was {}",
		a_event.m_key, a_event.m_began ? "Pressed" : a_event.m_ended ? "Released" : "Held");
}

void Simulation::On(const MouseButtonEvent& a_event) {
	Logger::Console()->info("Mouse Button Event: {} was {}",
		a_event.m_left ? "Left" : a_event.m_right ? "Right" : "Middle", a_event.m_pressed ? "Pressed" : "Released");
}

void Simulation::On(const MouseScrollEvent& a_event) {
	Logger::Console()->info("Mouse Scroll Event: {}", to_string(a_event.m_offset));
}

void Simulation::On(const MouseMovedEvent& a_event) {
	Logger::Console()->info("Mouse Position Event: {}", to_string(a_event.m_position));
}
