#include "SimState.h"
#include "Graphics.h"

Time SimState::s_delta;
Time SimState::s_global;

const Time& SimState::Delta() {
	return s_delta;
}

const Time& SimState::Global() {
	return s_global;
}

void SimState::Update() {
	const Time last = s_global;
	s_global = Graphics::GetGlobalTime();
	s_delta = s_global - last;
}
