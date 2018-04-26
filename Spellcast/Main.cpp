#include "Graphics.h"
#include "Camera.h"
#include "ContentManager.h"
#include "Simulation.h"

#include <glm/glm.hpp>
#include "DirectionLight.h"

using namespace glm;
using namespace std;

int main() {
	Graphics& graphics = Graphics::Instance();
	if (!graphics.Initialize("Spellcast")) exit(-1);

	Simulation& simulation = Simulation::Instance();
	if (!simulation.Initialize()) exit(-1);

	ContentManager::GetEntityDesc("Skybox.entity.json")->Create();
	ContentManager::GetEntityDesc("Camera.entity.json")->Create();
	
	ContentManager::GetEntityDesc("Floor.entity.json")->Create();
	ContentManager::GetEntityDesc("Boulder.entity.json")->Create()->GetTransform().Translate({0.f, -1.f, 0.f});
	ContentManager::GetEntityDesc("Boulder.entity.json")->Create()->GetTransform().Translate({0.f, 1.f, 0.f});
	ContentManager::GetEntityDesc("Boulder.entity.json")->Create()->GetTransform().Scale(0.5f);

	ContentManager::GetEntityDesc("Sun.entity.json")->Create();

	Time globalTime;
	while (!graphics.WindowClosed()) {
		const Time lastTime = globalTime;
		globalTime = Graphics::GetGlobalTime();
		const Time deltaTime = globalTime - lastTime;
		
		simulation.Update(deltaTime, globalTime);
		graphics.Update(deltaTime, globalTime);
	}

	return 0;
}
