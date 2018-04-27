#pragma once

#include "Listener.h"

#include <array>
#include <vector>

template <typename E>
class Listener;

template <typename E>
class EventManager {
public:
	static void Fire(const E& a_event);
	static void Register(Listener<E>* a_listener);
	static void Deregister(Listener<E>* a_listener);

private:
	static std::vector<Listener<E>*> s_listeners;
};

template <typename E>
std::vector<Listener<E>*> EventManager<E>::s_listeners;

template <typename E>
void EventManager<E>::Fire(const E& a_event) {
	for (Listener<E>* listener : s_listeners) {
		listener->On(a_event);
	}
}

template <typename E>
void EventManager<E>::Register(Listener<E>* a_listener) {
	s_listeners.push_back(a_listener);
}

template <typename E>
void EventManager<E>::Deregister(Listener<E>* a_listener) {
	s_listeners.erase(std::remove(s_listeners.begin(), s_listeners.end(), a_listener));
}
