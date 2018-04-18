#pragma once

#include "System.h"
#include "LightingShader.h"
#include "MeshRenderer.h"

struct GLFWwindow;

class GraphicsSystem : public System {
	GraphicsSystem();
public:
	GraphicsSystem(const GraphicsSystem&) = delete;
	GraphicsSystem& operator= (const GraphicsSystem&) = delete;

	~GraphicsSystem() override;
	static GraphicsSystem& Instance();

	bool Initialize(const std::string& a_windowTitle);
	void Update(const Time& a_deltaTime, const Time& a_globalTime) override;

	bool WindowClosed() const;
	static Time GetGlobalTime();

private:
	MeshRenderer m_meshRenderer;

	GLFWwindow* m_window;
	glm::vec2 m_windowDims;

	bool m_devToolsEnabled;
	size_t m_frameCount;
	Time m_lastTime;
	double m_framesPerSecond;

	LightingShader m_lighting;

	void RenderDevTools(const Time& a_globalTime);
};
