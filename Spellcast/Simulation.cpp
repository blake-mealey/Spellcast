#include "Simulation.h"
#include "World.h"
#include "Geometry.h"
#include "Entity.h"
#include "Camera.h"

#include <glm/glm.hpp>

using namespace glm;

Simulation::Simulation() = default;
Simulation::~Simulation() = default;

Simulation& Simulation::Instance() {
	static Simulation instance;
	return instance;
}

bool Simulation::Initialize() {
	return true;
}

void Simulation::Update(const Time& a_deltaTime, const Time& a_globalTime) {
	// Rotate entities... for reasons
	for (auto it = World::BeginEntities(); it != World::EndEntities(); ++it) {
		// it->GetTransform().Rotate(Geometry::UP, 0.01f);
	}

	constexpr float distance = 10.f;
	constexpr float speed = 0.3f;
	for (auto it = World::BeginComponents<Camera>(); it != World::EndComponents<Camera>(); ++it) {
		it->SetGlobalPosition(distance * vec3(
			cos(a_globalTime.GetSeconds()*speed), 0.1f, sin(a_globalTime.GetSeconds()*speed)));
	}
}
