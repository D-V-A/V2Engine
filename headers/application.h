#pragma once

#include <SDL3/SDL.h>

#include "renderer.h"
#include "world.h"
#include "player.h"
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

	SDL_Window* m_window = nullptr;
	Renderer m_renderer;
	World m_world;
	Player m_player;
	Input m_input;

	bool m_isRunning = false;
};