#pragma once

#include "System.h"

struct GLFWwindow;

class GraphicsSystem : public System {
	GraphicsSystem();
public:
	GraphicsSystem(const GraphicsSystem&) = delete;
	GraphicsSystem& operator= (const GraphicsSystem&) = delete;

	~GraphicsSystem() override;
	static GraphicsSystem& Instance();

	bool Initialize(const std::string& a_windowName);
	void Update(const Time& a_deltaTime, const Time& a_globalTime) override;

	bool WindowClosed();
	Time GetGlobalTime();

private:
	GLFWwindow* m_window;
};
