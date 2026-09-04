#pragma once

#include <SDL3/SDL.h>

#include "renderer.h"
#include "player.h"
#include "camera.h"
#include "window.h"
#include "world.h"
#include "input.h"


class Application
{
public:
	Application() = default;
	~Application();

	int Run();

private:
	bool Initialize();

	void ProcessEvents();
	void Update(float deltaTime);

	void Render();

	Window m_window;
	Renderer m_renderer;
	World m_world;
	Player m_player;
	Input m_input;
	Camera m_camera;

	bool m_isRunning = false;
};