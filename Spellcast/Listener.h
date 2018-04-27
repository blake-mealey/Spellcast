#pragma once

#include "EventManager.h"

template <typename E>
class Listener {
public:
	virtual ~Listener() { EventManager<E>::Deregister(this); }
	Listener() { EventManager<E>::Register(this); }

	virtual void On(const E& a_event) = 0;
};
