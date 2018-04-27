#pragma once

#include "System.h"
#include "KeyboardEvent.h"
#include "Listener.h"
#include "Time.h"

#include <glm/glm.hpp>

#define INITIAL_SCREEN_WIDTH 1024
#define INITIAL_SCREEN_HEIGHT 768

class LightingShader;
struct GLFWwindow;

class Graphics : public System, public Listener<KeyboardEvent> {
	Graphics();
public:
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;

	~Graphics() override;
	static Graphics& Instance();

	static void WindowSizeCallback(GLFWwindow* a_window, int a_width, int a_height);

	bool Init(const std::string& a_windowTitle);
	void Update() override;

	bool WindowClosed() const;
	static Time GetGlobalTime();

	const glm::vec2& GetWindowDims() const;
	glm::vec2 GetWindowCentre() const;
	GLFWwindow* GetWindow() const;

	void On(const KeyboardEvent& a_event) override;

private:
	void RenderDevTools();

	void SetWindowDims(const glm::vec2& a_windowDims);

	LightingShader* m_lightingShader;

	GLFWwindow* m_window;
	glm::vec2 m_windowDims;

	bool m_devToolsEnabled;
	size_t m_frameCount;
	Time m_lastTime;
	double m_framesPerSecond;
};
