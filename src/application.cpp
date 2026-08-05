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
	constexpr float speed = 200.0f; // пикселей в секунду

	rectangle.x += speed * deltaTime;

	if (rectangle.x > 1280.0f)
	{
		rectangle.x = -rectangle.w;
	}
}

void Application::Render()
{
	ProcessEvents();
	renderer.Clear();
	renderer.FillRect(rectangle);
	renderer.Present();
}