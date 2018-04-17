#include "GraphicsSystem.h"

#include "ContentManager.h"

using namespace std;

int main() {
	GraphicsSystem& graphics = GraphicsSystem::Instance();
	if (!graphics.Initialize("Spellcast")) exit(-1);

	Time globalTime;
	while (!graphics.WindowClosed()) {
		const Time lastTime = globalTime;
		globalTime = GraphicsSystem::GetGlobalTime();
		const Time deltaTime = globalTime - lastTime;

		graphics.Update(deltaTime, globalTime);
	}

	return 0;
}
