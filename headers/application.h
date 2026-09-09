#pragma once

#include <SDL3/SDL.h>

#include "text_renderer.h"
#include "sdl_context.h"
#include "renderer.h"
#include "player.h"
#include "camera.h"
#include "window.h"
#include "world.h"
#include "input.h"
#include "font.h"
#include "text.h"


class Application
{
public:
	int Run();

private:
	bool Initialize();

	void ProcessEvents();
	void Update(float deltaTime);
	void MovePlayer(float deltaTime);
	void UpdatePlayerViewDirection();

	void Render();

	SDLContext m_sdl;
	TTFContext m_ttf;

	Window m_window;
	Renderer m_renderer;
	TextRenderer m_textRenderer;

	World m_world;
	Player m_player;
	Input m_input;
	Camera m_camera;
	Font m_fonts;
	Text m_testText;

	bool m_isRunning = false;
};