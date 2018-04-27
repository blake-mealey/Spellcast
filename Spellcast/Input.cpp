#include "Input.h"
#include "Graphics.h"
#include "KeyboardEvent.h"
#include "MouseButtonEvent.h"
#include "EventManager.h"

#include <GLFW/glfw3.h>
#include "MouseScrollEvent.h"
#include "MouseMovedEvent.h"

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

	return true;
}

void Input::Update(const Time& a_deltaTime, const Time& a_globalTime) {
	glfwPollEvents();
}

void Input::KeyCallback(GLFWwindow* a_window, int a_key, int a_scanCode, int a_action, int a_mods) {
	EventManager<KeyboardEvent>::Fire({
		a_key,
		a_action == GLFW_PRESS,
		a_action == GLFW_RELEASE,
		a_action == GLFW_REPEAT,
		bool(a_mods | GLFW_MOD_SHIFT),
		bool(a_mods | GLFW_MOD_CONTROL),
		bool(a_mods | GLFW_MOD_ALT),
		bool(a_mods | GLFW_MOD_SUPER)
	});
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
