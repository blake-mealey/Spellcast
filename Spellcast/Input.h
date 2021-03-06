#pragma once

#include "System.h"
#include "KeyCode.h"

#include <glm/glm.hpp>

struct GLFWwindow;

class Input : public System {
	Input();
public:
	Input(const Input&) = delete;
	Input& operator= (const Input&) = delete;

	~Input() override;
	static Input& Instance();

	bool Init();
	void Update() override;

	void ResetMousePosition();
	void SetMousePosition(const glm::vec2& a_position);

	bool KeyDown(key_index a_key);
	bool MouseDown(mouse_index a_button);

	static void KeyCallback(GLFWwindow* a_window, int a_key, int a_scanCode, int a_action, int a_mods);
	static void MouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods);
	static void ScrollCallback(GLFWwindow* a_window, double a_xOffset, double a_yOffset);
	static void CursorPosCallback(GLFWwindow* a_window, double a_xPos, double a_yPos);

private:
	glm::vec2 m_mousePosition;
	bool m_keyDown[KEY_LAST];
	bool m_mouseDown[MouseButton::COUNT];
};
