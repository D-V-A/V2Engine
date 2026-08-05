#include "application.h"

#include <iostream>
#include <algorithm>

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
	MoveRectangle(deltaTime);

	ClampRectangleToWindow();
}

void Application::MoveRectangle(float deltaTime)
{
	constexpr float speed = 200.0f; // пикселей в секунду

	const bool* keyboardState = SDL_GetKeyboardState(nullptr);

	if (keyboardState[SDL_SCANCODE_W])
	{
		rectangle.y -= speed * deltaTime;
	}

	if (keyboardState[SDL_SCANCODE_S])
	{
		rectangle.y += speed * deltaTime;
	}

	if (keyboardState[SDL_SCANCODE_A])
	{
		rectangle.x -= speed * deltaTime;
	}

	if (keyboardState[SDL_SCANCODE_D])
	{
		rectangle.x += speed * deltaTime;
	}
}

void Application::ClampRectangleToWindow()
{
	int windowWidth;
	int windowHeight;

	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	rectangle.x = std::clamp(
		rectangle.x,
		0.0f,
		windowWidth - rectangle.w);
	rectangle.y = std::clamp(
		rectangle.y,
		0.0f,
		windowHeight - rectangle.h);
}

void Application::Render()
{
	ProcessEvents();
	renderer.Clear();
	renderer.FillRect(rectangle);
	renderer.Present();
}