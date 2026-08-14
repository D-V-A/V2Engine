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

	SDL_Window* window = nullptr;
	Renderer renderer;
	World world;
	Player player;
	Input input;

	bool isRunning = false;
};