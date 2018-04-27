#pragma once

struct MouseButtonEvent {
	bool m_left;
	bool m_right;
	bool m_middle;

	bool m_pressed;
	bool m_released;

	bool m_shiftHeld;
	bool m_ctrlHeld;
	bool m_altHeld;
	bool m_superHeld;
};