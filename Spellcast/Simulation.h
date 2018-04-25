#pragma once

#include "System.h"

class Simulation : public System {
	Simulation();
public:
	Simulation(const Simulation&) = delete;
	Simulation& operator= (const Simulation&) = delete;

	~Simulation() override;
	static Simulation& Instance();

	bool Initialize();
	void Update(const Time& a_deltaTime, const Time& a_globalTime) override;
};