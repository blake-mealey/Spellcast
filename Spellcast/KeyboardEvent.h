#pragma once

#include "KeyCode.h"

struct KeyboardEvent {
	key_code m_key;

	bool m_began;
	bool m_ended;
	bool m_changed;

	bool m_shiftHeld;
	bool m_ctrlHeld;
	bool m_altHeld;
	bool m_superHeld;
};
