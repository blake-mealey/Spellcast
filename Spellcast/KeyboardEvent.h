#pragma once

#include "KeyCode.h"

struct KeyboardEvent {
	key_code m_key;

	bool m_pressed;
	bool m_released;
	bool m_repeated;

	bool m_shiftHeld;
	bool m_ctrlHeld;
	bool m_altHeld;
	bool m_superHeld;
};
