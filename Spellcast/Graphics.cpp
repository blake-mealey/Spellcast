#include "Graphics.h"
#include "Geometry.h"
#include "Logger.h"
#include "Camera.h"
#include "LightingShader.h"
#include "DirectionLight.h"
#include "ContentManager.h"
#include "SpotLight.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include "SimState.h"

using namespace std;
using namespace glm;

#define OPENGL_DEBUG 1

Graphics& Graphics::Instance() {
	static Graphics instance;
	return instance;
}

Graphics::Graphics() : m_lightingShader(nullptr),
                       m_window(nullptr),
                       m_devToolsEnabled(true),
                       m_frameCount(0),
                       m_framesPerSecond(0) { }

Graphics::~Graphics() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Graphics::WindowSizeCallback(GLFWwindow* a_window, int a_width, int a_height) {
	Instance().SetWindowDims(vec2(a_width, a_height));
}

bool Graphics::Init(const string& a_windowTitle) {
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

	// Enable debug outputs if requested
#if OPENGL_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(Logger::GlErrorCallback, nullptr);
#endif

	// Initialize ImGui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfwGL3_Init(m_window, false);
	io.NavFlags |= ImGuiNavFlags_EnableGamepad | ImGuiNavFlags_EnableKeyboard;
	ImGui::StyleColorsDark();

	// Get the lighting shader
	m_lightingShader = static_cast<LightingShader*>(ContentManager::GetShaderProgram("Lighting"));

	return true;
}

void Graphics::Update() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render shadow maps
	for (auto it = World::BeginComponents<DirectionLight>(); it != World::EndComponents<DirectionLight>(); ++it) {
		it->RenderShadowMap();
	}
	for (auto it = World::BeginComponents<SpotLight>(); it != World::EndComponents<SpotLight>(); ++it) {
		it->RenderShadowMap();
	}

	// Load the current lights into the lighting shader
	m_lightingShader->LoadLights();

	// Render cameras
	for (auto it = World::BeginComponents<Camera>(); it != World::EndComponents<Camera>(); ++it) {
		it->Render(Instance());
	}

	RenderDevTools();

	glfwSwapBuffers(m_window);
}

bool Graphics::WindowClosed() const {
	return glfwWindowShouldClose(m_window);
}

Time Graphics::GetGlobalTime() {
	return glfwGetTime();
}

const vec2& Graphics::GetWindowDims() const {
	return m_windowDims;
}

vec2 Graphics::GetWindowCentre() const {
	return m_windowDims * 0.5f;
}

GLFWwindow* Graphics::GetWindow() const {
	return m_window;
}

void Graphics::On(const KeyboardEvent& a_event) {
	if (IS_KEY(a_event.m_key, F4) && a_event.m_began) {
		glfwSetWindowShouldClose(m_window, 1);
	}
}

void Graphics::RenderDevTools() {
	ImGui_ImplGlfwGL3_NewFrame();

	if (m_devToolsEnabled) {
		ImGui::Begin("Dev Tools", &m_devToolsEnabled);

		m_frameCount++;
		if (SimState::Global() - m_lastTime >= 1.0) {
			m_framesPerSecond = double(m_frameCount);
			m_frameCount = 0;
			m_lastTime = SimState::Global();
		}

		ImGui::LabelText("FPS", "%.0f", m_framesPerSecond);
		ImGui::LabelText("ms/frame", "%.3f", 1000.0 / m_framesPerSecond);

		ImGui::End();
	}

	glViewport(0, 0, m_windowDims.x, m_windowDims.y);
	ImGui::Render();
}

void Graphics::SetWindowDims(const vec2& a_windowDims) {
	m_windowDims = a_windowDims;
}
