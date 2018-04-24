#include "GraphicsSystem.h"
#include "Geometry.h"
#include "Logger.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

using namespace std;
using namespace glm;

#define OPENGL_DEBUG 0

#define INITIAL_SCREEN_WIDTH 1024
#define INITIAL_SCREEN_HEIGHT 768

GraphicsSystem& GraphicsSystem::Instance() {
	static GraphicsSystem instance;
	return instance;
}

GraphicsSystem::GraphicsSystem() :
	m_window(nullptr),
	m_devToolsEnabled(true),
	m_frameCount(0),
	m_framesPerSecond(0) { }

GraphicsSystem::~GraphicsSystem() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void GraphicsSystem::WindowSizeCallback(GLFWwindow* a_window, int a_width, int a_height) {
	Instance().SetWindowDims(vec2(a_width, a_height));
}

bool GraphicsSystem::Initialize(const string& a_windowTitle) {
	// Initialize GLFW
	if (!glfwInit()) {
		Logger::Console()->error("Could not initialize GLFW.");
		return false;
	}

	// Create GLFW window
#if OPENGL_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	m_window = glfwCreateWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, a_windowTitle.c_str(), nullptr, nullptr);
	if (!m_window) {
		Logger::Console()->error("Could not initialize GLFW window.");
		return false;
	}

	// Initialize window
	glfwMakeContextCurrent(m_window);
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	glfwSwapInterval(1);
	m_windowDims = vec2(width, height);

	// TODO: GLFW input callbacks
	//glfwSetMouseButtonCallback()

	// Window callbacks
	glfwSetWindowSizeCallback(m_window, WindowSizeCallback);

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

	// Enable debug outputs
#if OPENGL_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(Logger::GlErrorCallback, nullptr);
#endif

	// Initialize ImGui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfwGL3_Init(m_window, true);
	io.NavFlags |= ImGuiNavFlags_EnableGamepad | ImGuiNavFlags_EnableKeyboard;
	ImGui::StyleColorsDark();

	CameraDesc c;
	c.m_globalPosition = vec3(0.f, 0.f, -5.f);
	m_camera.Init(c);

	return true;
}

void GraphicsSystem::Update(const Time& a_deltaTime, const Time& a_globalTime) {
	glfwPollEvents();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, m_windowDims.x, m_windowDims.y);

	m_camera.Render(Instance());

	RenderDevTools(a_globalTime);

	glfwSwapBuffers(m_window);
}

bool GraphicsSystem::WindowClosed() const {
	return glfwWindowShouldClose(m_window);
}

Time GraphicsSystem::GetGlobalTime() {
	return glfwGetTime();
}

const vec2& GraphicsSystem::GetWindowDims() const {
	return m_windowDims;
}

void GraphicsSystem::RenderDevTools(const Time& a_globalTime) {
	ImGui_ImplGlfwGL3_NewFrame();

	if (m_devToolsEnabled) {
		ImGui::Begin("Dev Tools", &m_devToolsEnabled);

		m_frameCount++;
		if (a_globalTime - m_lastTime >= 1.0) {
			m_framesPerSecond = double(m_frameCount);
			m_frameCount = 0;
			m_lastTime = a_globalTime;
		}

		ImGui::LabelText("FPS", "%.0f", m_framesPerSecond);
		ImGui::LabelText("ms/frame", "%.3f", 1000.0 / m_framesPerSecond);

		ImGui::End();
	}

	glViewport(0, 0, m_windowDims.x, m_windowDims.y);
	ImGui::Render();
}

void GraphicsSystem::SetWindowDims(const vec2& a_windowDims) {
	m_windowDims = a_windowDims;
}
