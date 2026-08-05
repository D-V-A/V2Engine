#include "application.h"

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

	return true;
}


int Application::Run()
{
	if (!Initialize())
		return 1;

	isRunning = true;

	constexpr Uint64 targetFrameTime = 1000 / 60;

	while (isRunning)
	{
		const Uint64 frameStart = SDL_GetTicks();

		ProcessEvents();
		//Update();
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

void Application::Render()
{
	// Прямоугольник в координатах окна.
	SDL_FRect rectangle{
		440.0f,
		260.0f,
		400.0f,
		200.0f
	};
	ProcessEvents();
	renderer.Clear();
	renderer.FillRect(rectangle);
	renderer.Present();
}