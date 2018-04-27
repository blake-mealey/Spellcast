#include "Graphics.h"
#include "Input.h"
#include "Simulation.h"
#include "ContentManager.h"

#include <glm/glm.hpp>
#include "SimState.h"

using namespace glm;
using namespace std;

int main() {
	// Initialize systems
	Graphics& graphics = Graphics::Instance();
	if (!graphics.Init("Spellcast")) exit(-1);

	Input& input = Input::Instance();
	if (!input.Init()) exit(-1);

	Simulation& simulation = Simulation::Instance();
	if (!simulation.Init()) exit(-1);

	// Order of execution
	vector<System*> systems = { &input, &simulation, &graphics };

	// Create entities
	ContentManager::GetEntityDesc("Skybox.entity.json")->Create();
	// ContentManager::GetEntityDesc("Camera.entity.json")->Create();
	ContentManager::GetEntityDesc("Player.entity.json")->Create()->GetTransform().Translate({});
	
	ContentManager::GetEntityDesc("Floor.entity.json")->Create();
	ContentManager::GetEntityDesc("Boulder.entity.json")->Create()->GetTransform().Translate({0.f, -1.f, 0.f});
	ContentManager::GetEntityDesc("Boulder.entity.json")->Create()->GetTransform().Translate({0.f, 1.f, 0.f});
	ContentManager::GetEntityDesc("Boulder.entity.json")->Create()->GetTransform().Scale(0.5f);

	ContentManager::GetEntityDesc("Sun.entity.json")->Create();
	auto spotLight = ContentManager::GetEntityDesc("SpotLight.entity.json")->Create();
	auto pointLight = ContentManager::GetEntityDesc("PointLight.entity.json")->Create();

	// Simulation loop
	while (!graphics.WindowClosed()) {
		// Update time
		SimState::Update();

		// Update systems
		for (System* system : systems) system->Update();

		// TODO: Remove
		spotLight->GetTransform().Rotate(vec3(0.f, 1.f, 0.f), 0.01f);
	}

	// Exit normally
	return 0;
}
