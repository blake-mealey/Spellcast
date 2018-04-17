#pragma once

#include "Time.h"

class System {
public:
	virtual ~System() = default;

	virtual void Update(const Time& a_deltaTime, const Time& a_globalTime) = 0;
};
