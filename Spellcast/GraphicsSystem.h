#pragma once

#include "System.h"
#include "LightingShader.h"
#include "MeshRenderer.h"
#include <GLFW/glfw3.h>
#include "Camera.h"

struct GLFWwindow;

class GraphicsSystem : public System {
	GraphicsSystem();
public:
	GraphicsSystem(const GraphicsSystem&) = delete;
	GraphicsSystem& operator= (const GraphicsSystem&) = delete;

	~GraphicsSystem() override;
	static GraphicsSystem& Instance();

	static void WindowSizeCallback(GLFWwindow* a_window, int a_width, int a_height);

	bool Initialize(const std::string& a_windowTitle);
	void Update(const Time& a_deltaTime, const Time& a_globalTime) override;

	bool WindowClosed() const;
	static Time GetGlobalTime();

	const glm::vec2& GetWindowDims() const;

private:
	void RenderDevTools(const Time& a_globalTime);

	void SetWindowDims(const glm::vec2& a_windowDims);

	// MeshRenderer m_meshRenderer;
	Camera m_camera;

	GLFWwindow* m_window;
	glm::vec2 m_windowDims;

	bool m_devToolsEnabled;
	size_t m_frameCount;
	Time m_lastTime;
	double m_framesPerSecond;

	LightingShader m_lighting;
};
