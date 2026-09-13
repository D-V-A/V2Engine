#pragma once

#include <SDL3/SDL.h>

#include "core/sdl_context.h"
#include "core/input.h"

#include "world/camera.h"
#include "world/world.h"

#include "graphics/text_renderer.h"
#include "graphics/renderer.h"
#include "graphics/window.h"
#include "graphics/font.h"

#include "ui/text_panel.h"

#include "entities/player.h"


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
	TextPanel m_testTextPanel;

	bool m_isRunning = false;
};