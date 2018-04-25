#pragma once

#include "System.h"

#include <glm/glm.hpp>

#define INITIAL_SCREEN_WIDTH 1024
#define INITIAL_SCREEN_HEIGHT 768

class LightingShader;
struct GLFWwindow;

class Graphics : public System {
	Graphics();
public:
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;

	~Graphics() override;
	static Graphics& Instance();

	static void WindowSizeCallback(GLFWwindow* a_window, int a_width, int a_height);

	bool Initialize(const std::string& a_windowTitle);
	void Update(const Time& a_deltaTime, const Time& a_globalTime) override;

	bool WindowClosed() const;
	static Time GetGlobalTime();

	const glm::vec2& GetWindowDims() const;

private:
	void RenderDevTools(const Time& a_globalTime);

	void SetWindowDims(const glm::vec2& a_windowDims);

	LightingShader* m_lightingShader;

	GLFWwindow* m_window;
	glm::vec2 m_windowDims;

	bool m_devToolsEnabled;
	size_t m_frameCount;
	Time m_lastTime;
	double m_framesPerSecond;
};
