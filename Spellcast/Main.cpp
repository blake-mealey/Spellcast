#include "GraphicsSystem.h"

#include "ContentManager.h"

using namespace std;

int main() {
	GraphicsSystem& graphics = GraphicsSystem::Instance();
	if (!graphics.Initialize("Spellcast")) exit(-1);

	Entity* b0 = ContentManager::GetEntityDesc("Boulder.entity.json")->Create();
	Entity* b1 = ContentManager::GetEntityDesc("Boulder.entity.json")->Create();
	Entity* b2 = ContentManager::GetEntityDesc("Boulder.entity.json")->Create();

	b0->GetTransform().Translate({0.f, -1.f, 0.f});
	b1->GetTransform().Translate({0.f, 1.f, 0.f});
	b2->GetTransform().Scale(0.5f);

	// ContentManager::GetEntityDesc("Boulder_all.entity.json")->Create();
	// World::GetEntity(ContentManager::GetEntityDesc("Boulder_all.entity.json")->Create())->GetTransform().Translate({2.f, 1.f, 0.f});
	// World::GetEntity(ContentManager::GetEntityDesc("Boulder.entity.json")->Create())->GetComponent<MeshRenderer>()->GetTransform().Translate({2.f, 1.f, 0.f});

	Time globalTime;
	while (!graphics.WindowClosed()) {
		const Time lastTime = globalTime;
		globalTime = GraphicsSystem::GetGlobalTime();
		const Time deltaTime = globalTime - lastTime;

		graphics.Update(deltaTime, globalTime);
	}

	return 0;
}
