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
using namespace glm;

Input::Input(): m_keyDown{}, m_mouseDown{} {}
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

void Input::Update() {
	glfwPollEvents();

	const bool shift = m_keyDown[KEY(LEFT_SHIFT)] || m_keyDown[KEY(RIGHT_SHIFT)];
	const bool ctrl = m_keyDown[KEY(LEFT_CONTROL)] || m_keyDown[KEY(RIGHT_CONTROL)];
	const bool alt = m_keyDown[KEY(LEFT_ALT)] || m_keyDown[KEY(RIGHT_ALT)];
	const bool super = m_keyDown[KEY(LEFT_SUPER)] || m_keyDown[KEY(RIGHT_SUPER)];

	// Fire key down events every frame
	for (key_code key : KeyCode::KEYS) {
		if (IS_KEY(key, UNKNOWN)) continue;
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

void Input::ResetMousePosition() {
	SetMousePosition(Graphics::Instance().GetWindowCentre());
}

void Input::SetMousePosition(const vec2& a_position) {
	m_mousePosition = a_position;
	glfwSetCursorPos(Graphics::Instance().GetWindow(), m_mousePosition.x, m_mousePosition.y);
}

bool Input::KeyDown(const key_index a_key) {
	return m_keyDown[KeyCode::KEYS[a_key]];
}

bool Input::MouseDown(const mouse_index a_button) {
	return m_mouseDown[MouseButton::BUTTONS[a_button]];
}

void Input::KeyCallback(GLFWwindow* a_window, const int a_key, int a_scanCode, const int a_action, const int a_mods) {
	// Too slow (we use custom continuous firing in Input::Update())
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

void Input::MouseButtonCallback(GLFWwindow* a_window, const int a_button, const int a_action, const int a_mods) {
	const bool pressed = a_action == GLFW_PRESS;
	EventManager<MouseButtonEvent>::Fire({
		a_button == GLFW_MOUSE_BUTTON_LEFT,
		a_button == GLFW_MOUSE_BUTTON_RIGHT,
		a_button == GLFW_MOUSE_BUTTON_MIDDLE,
		pressed,
		a_action == GLFW_RELEASE,
		bool(a_mods | GLFW_MOD_SHIFT),
		bool(a_mods | GLFW_MOD_CONTROL),
		bool(a_mods | GLFW_MOD_ALT),
		bool(a_mods | GLFW_MOD_SUPER),
		Instance().m_mousePosition
	});
	Instance().m_mouseDown[a_button] = pressed;
}

void Input::ScrollCallback(GLFWwindow* a_window, const double a_xOffset, const double a_yOffset) {
	EventManager<MouseScrollEvent>::Fire({ {a_xOffset, a_yOffset} });
}

void Input::CursorPosCallback(GLFWwindow* a_window, const double a_xPos, const double a_yPos) {
	const vec2 position = vec2(a_xPos, a_yPos);
	EventManager<MouseMovedEvent>::Fire({
		position,
		position - Instance().m_mousePosition,
		position - Graphics::Instance().GetWindowCentre(),
		Instance().m_mouseDown[MBUTTON(LEFT)],
		Instance().m_mouseDown[MBUTTON(RIGHT)],
		Instance().m_mouseDown[MBUTTON(MIDDLE)]
	});
	Instance().m_mousePosition = position;
}
