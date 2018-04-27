#pragma once

#include "System.h"
#include "KeyCode.h"

struct GLFWwindow;

class Input : public System {
	Input();
public:
	Input(const Input&) = delete;
	Input& operator= (const Input&) = delete;

	~Input() override;
	static Input& Instance();

	bool Init();
	void Update(const Time& a_deltaTime, const Time& a_globalTime) override;

	static void KeyCallback(GLFWwindow* a_window, int a_key, int a_scanCode, int a_action, int a_mods);
	static void MouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods);
	static void ScrollCallback(GLFWwindow* a_window, double a_xOffset, double a_yOffset);
	static void CursorPosCallback(GLFWwindow* a_window, double a_xPos, double a_yPos);

private:
	bool m_keyDown[KEY_LAST];
};
