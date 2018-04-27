#include "Input.h"
#include "Graphics.h"
#include "KeyboardEvent.h"
#include "MouseButtonEvent.h"
#include "EventManager.h"
#include "MouseScrollEvent.h"
#include "MouseMovedEvent.h"

#include <GLFW/glfw3.h>

#include <algorithm>

using namespace std;

Input::Input() = default;
Input::~Input() = default;

Input& Input::Instance() {
	static Input instance;
	return instance;
}

bool Input::Init() {
	GLFWwindow* window = Graphics::Instance().GetWindow();
	
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetCursorPosCallback(window, CursorPosCallback);

	fill(begin(m_keyDown), end(m_keyDown), false);

	return true;
}

void Input::Update(const Time& a_deltaTime, const Time& a_globalTime) {
	glfwPollEvents();

	const bool shift = m_keyDown[KEY(LEFT_SHIFT)] || m_keyDown[KEY(RIGHT_SHIFT)];
	const bool ctrl = m_keyDown[KEY(LEFT_CONTROL)] || m_keyDown[KEY(RIGHT_CONTROL)];
	const bool alt = m_keyDown[KEY(LEFT_ALT)] || m_keyDown[KEY(RIGHT_ALT)];
	const bool super = m_keyDown[KEY(LEFT_SUPER)] || m_keyDown[KEY(RIGHT_SUPER)];

	for (key_code key : KeyCode::KEYS) {
		if (m_keyDown[key]) {
			EventManager<KeyboardEvent>::Fire({
				key,
				false,
				false,
				true,
				shift,
				ctrl,
				alt,
				super
			});
		}
	}
}

void Input::KeyCallback(GLFWwindow* a_window, int a_key, int a_scanCode, int a_action, int a_mods) {
	if (a_action == GLFW_REPEAT) return;
	const bool began = a_action == GLFW_PRESS;
	EventManager<KeyboardEvent>::Fire({
		a_key,
		began,
		!began,
		false,
		bool(a_mods | GLFW_MOD_SHIFT),
		bool(a_mods | GLFW_MOD_CONTROL),
		bool(a_mods | GLFW_MOD_ALT),
		bool(a_mods | GLFW_MOD_SUPER)
	});
	Instance().m_keyDown[a_key] = began;
}

void Input::MouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods) {
	EventManager<MouseButtonEvent>::Fire({
		a_button == GLFW_MOUSE_BUTTON_LEFT,
		a_button == GLFW_MOUSE_BUTTON_RIGHT,
		a_button == GLFW_MOUSE_BUTTON_MIDDLE,
		a_action == GLFW_PRESS,
		a_action == GLFW_RELEASE,
		bool(a_mods | GLFW_MOD_SHIFT),
		bool(a_mods | GLFW_MOD_CONTROL),
		bool(a_mods | GLFW_MOD_ALT),
		bool(a_mods | GLFW_MOD_SUPER)
	});
}

void Input::ScrollCallback(GLFWwindow* a_window, double a_xOffset, double a_yOffset) {
	EventManager<MouseScrollEvent>::Fire({ {a_xOffset, a_yOffset} });
}

void Input::CursorPosCallback(GLFWwindow* a_window, double a_xPos, double a_yPos) {
	EventManager<MouseMovedEvent>::Fire({ {a_xPos, a_yPos} });
}
