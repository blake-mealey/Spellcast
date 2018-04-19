#include "GraphicsSystem.h"

#include "ContentManager.h"
#include <iostream>

using namespace std;

int main() {
	GraphicsSystem& graphics = GraphicsSystem::Instance();
	if (!graphics.Initialize("Spellcast")) exit(-1);

	ContentManager::GetEntityDesc("Boulder.entity.json")->Create();

	Time globalTime;
	while (!graphics.WindowClosed()) {
		const Time lastTime = globalTime;
		globalTime = GraphicsSystem::GetGlobalTime();
		const Time deltaTime = globalTime - lastTime;

		graphics.Update(deltaTime, globalTime);
	}

	return 0;
}
