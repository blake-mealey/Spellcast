#pragma once

#include "Time.h"

class SimState {
public:
	static const Time& Delta();
	static const Time& Global();
	
	static void Update();

private:
	static Time s_delta;
	static Time s_global;
};
