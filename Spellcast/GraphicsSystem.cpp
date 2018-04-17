#include "GraphicsSystem.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include <iostream>

using namespace std;

#define INITIAL_SCREEN_WIDTH 1024
#define INITIAL_SCREEN_HEIGHT 768

GraphicsSystem& GraphicsSystem::Instance() {
	static GraphicsSystem instance;
	return instance;
}

GraphicsSystem::GraphicsSystem() : m_window(nullptr) { }

GraphicsSystem::~GraphicsSystem() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool GraphicsSystem::Initialize(const string& a_windowTitle) {
	// Initialize GLFW
	if (!glfwInit()) {
		cerr << "Error initializing GLFW." << endl;
		return false;
	}

	// Create GLFW window
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	m_window = glfwCreateWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, a_windowTitle.c_str(), nullptr, nullptr);
	if (!m_window) {
		cerr << "Error creating GLFW window." << endl;
		return false;
	}

	// Initialize window
	glfwMakeContextCurrent(m_window);
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	glfwSwapInterval(1);

	// TODO: GLFW input callbacks
	//glfwSetMouseButtonCallback()

	// Center window on primary monitor
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	const int xPos = (mode->width - INITIAL_SCREEN_WIDTH) / 2;
	const int yPos = (mode->height - INITIAL_SCREEN_HEIGHT) / 2;
	glfwSetWindowPos(m_window, xPos, yPos);

	// Initialize GLEW
	glewInit();

	// Initialize OpenGL
	glClearColor(11.f / 255.f, 11.f / 255.f, 110 / 255.f, 1.f);
	glLineWidth(2.f);

	// Enable z-buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	// Enable back-face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Initialize ImGui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfwGL3_Init(m_window, true);
	io.NavFlags |= ImGuiNavFlags_EnableGamepad | ImGuiNavFlags_EnableKeyboard;
	ImGui::StyleColorsDark();

	return true;
}

void GraphicsSystem::Update(const Time& a_deltaTime, const Time& a_globalTime) {
	glfwPollEvents();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSwapBuffers(m_window);
}

bool GraphicsSystem::WindowClosed() {
	return glfwWindowShouldClose(m_window);
}

Time GraphicsSystem::GetGlobalTime() {
	return glfwGetTime();
}
