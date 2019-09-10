#include "Graphics.h"
#include "Input.h"
#include "Simulation.h"

#include <glm/glm.hpp>
#include "SimState.h"
#include "Geometry.h"
#include "SpotLight.h"
#include "Camera.h"
#include "InjectedContentManager.h"

using namespace glm;
using namespace std;

int main() {
    InjectedContentManager contentManager;

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
	contentManager.GetEntityDesc("Skybox.entity.json")->Create();
	contentManager.GetEntityDesc("Player.entity.json")->Create()->GetTransform().Translate({});
	
	contentManager.GetEntityDesc("Floor.entity.json")->Create();
	contentManager.GetEntityDesc("Boulder.entity.json")->Create()->GetTransform().Translate(-Geometry::UP);
	contentManager.GetEntityDesc("Boulder.entity.json")->Create()->GetTransform().Translate(Geometry::UP);
	contentManager.GetEntityDesc("Boulder.entity.json")->Create()->GetTransform().Scale(0.5f);

	contentManager.GetEntityDesc("Sun.entity.json")->Create();
	contentManager.GetEntityDesc("PointLight.entity.json")->Create()->GetTransform().SetPosition(-Geometry::UP * 10.f);
	
	contentManager.GetEntityDesc("CubeTerrain.entity.json")->Create();

	EntityDesc* spotLightDesc = contentManager.GetEntityDesc("SpotLight.entity.json");
	constexpr int count = 10;
	constexpr float distance = 3.f;
	for (int i = 0; i < count; ++i) {
		Entity* spotLight = spotLightDesc->Create();
		const float angle = float(i) / float(count) * G_2_PI;
		const vec3 position = distance * vec3(cos(angle), 2.f / distance, sin(angle));
		spotLight->GetTransform().SetPosition(position);
	}


	// Simulation loop
	while (!graphics.WindowClosed()) {
		// Update time
		SimState::Update();

		// TODO: Remove/move
		for (auto it = World::BeginComponents<SpotLight>(); it != World::EndComponents<SpotLight>(); ++it) {
			Transform& transform = it->GetEntity()->GetTransform();
			const vec3 position = {
				transform.GetLocalPosition().x,
				(2.f + sin(SimState::Global().GetSeconds())),
				transform.GetLocalPosition().z
			};
			transform.SetPosition(position);
			it->SetDirection(-position);
			transform.Rotate(Geometry::UP, 0.02f);
		}

		// Update systems
		for (System* system : systems) system->Update();
	}

	// Exit normally
	return 0;
}
