#pragma once

#include <SDL3/SDL.h>

#include "sdl_context.h"
#include "renderer.h"
#include "player.h"
#include "camera.h"
#include "window.h"
#include "world.h"
#include "input.h"


class Application
{
public:
	int Run();

private:
	bool Initialize();

	void ProcessEvents();
	void Update(float deltaTime);
	inline void MovePlayer(float deltaTime);
	inline void UpdatePlayerViewDirectrion();

	void Render();

	SDLContext m_sdl;
	Window m_window;
	Renderer m_renderer;
	World m_world;
	Player m_player;
	Input m_input;
	Camera m_camera;

	bool m_isRunning = false;
};