#include <iostream>
#include <vector>

#include "application.h"
#include "isometric.h"
#include "render_order.h"
#include "assets.h"

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

	std::filesystem::path asset_path = GetAssetPath("map/map.txt");
	InitializationResults world_init_res = m_world.Initialize(m_renderer, asset_path.string().c_str());
	if (world_init_res != InitializationResults::Success)
	{
		switch (world_init_res)
		{
		case(InitializationResults::InfoLoadFail):
			std::cerr << "Failed to load map info: " << asset_path << '\n';
			break;
		case(InitializationResults::MapInitFail):
			std::cerr << "Failed to init map: " << SDL_GetError() << '\n';
			break;
		case(InitializationResults::ObjTxtFail):
			std::cerr << "Failed to init object model: " << SDL_GetError() << '\n';
			break;
		}

		return false;
	}

	asset_path = GetAssetPath("txt/player.png");
	if (!m_player.Initialize(m_renderer, asset_path.string().c_str()))
	{
		std::cerr << "Failed to init player model: "
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

	std::vector<const Entity*> objectQueue;
	objectQueue.reserve(m_world.GetObjectsList().size() + 1);
	objectQueue.push_back(&m_player);

	for (const WorldObject& object : m_world.GetObjectsList())
	{
		objectQueue.push_back(&object);
	}

	const std::vector<const Entity*> sortedQueue = BuildRenderOrder(objectQueue);

	for (const Entity* entity : sortedQueue)
	{
		const Vector2f screenPosition = WorldToScreen(entity->GetPosition(),{ m_world.GetTileWidth(),m_world.GetTileHeight() },	m_world.GetOrigin());

		entity->Render(m_renderer, screenPosition);
	}
	m_renderer.Present();
}