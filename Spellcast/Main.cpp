#include "GraphicsSystem.h"
#include "Camera.h"
#include "ContentManager.h"

#include <glm/glm.hpp>

using namespace glm;
using namespace std;

int main() {
	GraphicsSystem& graphics = GraphicsSystem::Instance();
	if (!graphics.Initialize("Spellcast")) exit(-1);

	ContentManager::GetEntityDesc("Skybox.entity.json")->Create();
	
	Entity* c0 = ContentManager::GetEntityDesc("Camera.entity.json")->Create();
	c0->GetComponent<Camera>()->SetViewportUnitScale(vec2(0.5f, 1.f));
	Entity* c1 = ContentManager::GetEntityDesc("Camera.entity.json")->Create();
	c1->GetComponent<Camera>()->SetViewportUnitScale(vec2(0.5f, 1.f));
	c1->GetComponent<Camera>()->SetViewportUnitPosition(vec2(0.5f, 0.f));

	ContentManager::GetEntityDesc("Boulder.entity.json")->Create()->GetTransform().Translate({0.f, -1.f, 0.f});
	ContentManager::GetEntityDesc("Boulder.entity.json")->Create()->GetTransform().Translate({0.f, 1.f, 0.f});
	ContentManager::GetEntityDesc("Boulder.entity.json")->Create()->GetTransform().Scale(0.5f);

	Time globalTime;
	while (!graphics.WindowClosed()) {
		const Time lastTime = globalTime;
		globalTime = GraphicsSystem::GetGlobalTime();
		const Time deltaTime = globalTime - lastTime;

		graphics.Update(deltaTime, globalTime);
	}

	return 0;
}
