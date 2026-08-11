#include "application.h"
#include "isometric.h"

#include <iostream>

Application::~Application()
{
	if (window != nullptr)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}
	SDL_Quit();
}

bool Application::Initialize()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cerr << "Failed to initialize SDL: "
			<< SDL_GetError() << '\n';

		return false;
	}

	window = SDL_CreateWindow(
		"V2Engine",
		1280,
		720,
		0
	);

	if (window == nullptr)
	{
		std::cerr << "Failed to create window: "
			<< SDL_GetError() << '\n';

		return false;
	}
	
	if (!renderer.Initialize(window))
	{
		std::cerr << "Failed to create renderer: "
			<< SDL_GetError() << '\n';

		return false;
	}

	if (!world.Initialize(renderer))
	{
		std::cerr << "Failed to init world: "
			<< SDL_GetError() << '\n';

		return false;
	}

	if (!player.Initialize(renderer))
	{
		std::cerr << "Failed to init player model: "
			<< SDL_GetError() << '\n';

		return false;
	}

	if (!worldObject.Initialize(renderer, "assets/txt/crate.bmp"))
	{
		std::cerr << "Failed to init object model: "
			<< SDL_GetError() << '\n';

		return false;
	}

	return true;
}


int Application::Run()
{
	if (!Initialize())
		return 1;

	isRunning = true;

	constexpr Uint64 targetFrameTime = 1000 / 60;
	Uint64 previousTime = SDL_GetTicks();

	while (isRunning)
	{
		const Uint64 frameStart = SDL_GetTicks();
		const float deltaTime =
			static_cast<float>(frameStart - previousTime) / 1000.0f;

		previousTime = frameStart;

		ProcessEvents();
		Update(deltaTime);
		Render();		

		const Uint64 frameTime = SDL_GetTicks() - frameStart;

		if (frameTime < targetFrameTime)
		{
			SDL_Delay(static_cast<Uint32>(targetFrameTime - frameTime));
		}
	}

	return 0;
}

void Application::ProcessEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			isRunning = false;
		}
	}
}

void Application::Update(float deltaTime)
{
	input.Update();
	player.MovePlayer(deltaTime, input.GetDirection());
	player.CheckMapBorders(world.GetSize());
}

void Application::Render()
{
	renderer.Clear();

	world.Render(renderer);

	Vector2f screenPosition = WorldToScreen(player.GetPosition(), { world.GetTileWidth(),world.GetTileHeight() }, world.GetOrigin());
	player.Render(renderer, screenPosition);
	screenPosition = WorldToScreen(worldObject.GetPosition(), { world.GetTileWidth(),world.GetTileHeight() }, world.GetOrigin());
	worldObject.Render(renderer, screenPosition);
	renderer.Present();
}