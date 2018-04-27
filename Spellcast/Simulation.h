#pragma once

#include "System.h"
#include "Listener.h"
#include "KeyboardEvent.h"
#include "MouseButtonEvent.h"
#include "MouseScrollEvent.h"
#include "MouseMovedEvent.h"

class Simulation : public System,
                   public Listener<KeyboardEvent>,
                   public Listener<MouseMovedEvent>,
                   public Listener<MouseScrollEvent>,
                   public Listener<MouseButtonEvent> {
	Simulation();
public:
	Simulation(const Simulation&) = delete;
	Simulation& operator= (const Simulation&) = delete;

	~Simulation() override;
	static Simulation& Instance();

	bool Init();
	void Update(const Time& a_deltaTime, const Time& a_globalTime) override;

	void On(const KeyboardEvent& a_event) override;
	void On(const MouseButtonEvent& a_event) override;
	void On(const MouseScrollEvent& a_event) override;
	void On(const MouseMovedEvent& a_event) override;
};
