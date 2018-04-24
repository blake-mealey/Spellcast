#include "GraphicsSystem.h"

#include "ContentManager.h"

using namespace std;

int main() {
	GraphicsSystem& graphics = GraphicsSystem::Instance();
	if (!graphics.Initialize("Spellcast")) exit(-1);

	ContentManager::GetEntityDesc("Skybox.entity.json")->Create();

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
