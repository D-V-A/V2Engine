#include <iostream>
#include <algorithm>

#include "application.h"
#include "isometric.h"

Application::~Application()
{
	if (m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
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

	m_window = SDL_CreateWindow(
		"V2Engine",
		1280,
		720,
		0
	);

	if (m_window == nullptr)
	{
		std::cerr << "Failed to create window: "
			<< SDL_GetError() << '\n';

		return false;
	}
	
	if (!m_renderer.Initialize(m_window))
	{
		std::cerr << "Failed to create renderer: "
			<< SDL_GetError() << '\n';

		return false;
	}

	if (!m_world.Initialize(m_renderer))
	{
		std::cerr << "Failed to init world: "
			<< SDL_GetError() << '\n';

		return false;
	}

	if (!m_player.Initialize(m_renderer, "assets/txt/player.bmp"))
	{
		std::cerr << "Failed to init player model: "
			<< SDL_GetError() << '\n';

		return false;
	}

	if (!m_world.InitializeObjects(m_renderer, "assets/txt/crate.bmp"))
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

	m_isRunning = true;

	constexpr Uint64 targetFrameTime = 1000 / 60;
	Uint64 previousTime = SDL_GetTicks();

	while (m_isRunning)
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
			m_isRunning = false;
		}
	}
}

void Application::Update(float deltaTime)
{
	m_input.Update();
	Vector2i movementDirection = m_input.GetDirection();
	if (movementDirection.x == 0 && movementDirection.y == 0) 
	{
		return;
	}
	Vector2f movement = m_player.CalculateMovement(deltaTime, movementDirection);
	Rect collisionRect = m_player.GetCollisionRect();
	movement = m_world.ResolveMovement(collisionRect, movement);
	m_player.MovePlayer(movement);
}

void Application::Render()
{
	m_renderer.Clear();

	m_world.Render(m_renderer);

	std::vector<Entity*> renderQueue;
	renderQueue.push_back(&m_player);

	for (WorldObject& object : m_world.GetObjectsList())
	{
		renderQueue.push_back(&object);
	}

	std::sort(renderQueue.begin(), renderQueue.end(), [](const Entity* a, const Entity* b)
		{return a->GetDepth() < b->GetDepth(); });

	for (Entity* entity : renderQueue)
	{
		const Vector2f screenPosition = WorldToScreen(entity->GetPosition(),{ m_world.GetTileWidth(),m_world.GetTileHeight() },	m_world.GetOrigin());

		entity->Render(m_renderer, screenPosition);
	}
	m_renderer.Present();
}