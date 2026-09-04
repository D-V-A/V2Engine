#include <iostream>
#include <vector>

#include "application.h"
#include "isometric.h"
#include "render_order.h"
#include "assets.h"

Application::~Application()
{
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

	if (!m_window.Initialize("V2Engine", 1280, 720))
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

	std::filesystem::path asset_path = GetAssetPath("map/map_20_30.txt");
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

	const Vector2i movementDirection = m_input.GetDirection();

	if (movementDirection.x == 0 && movementDirection.y == 0)
		return;

	const float speedModifier =	m_world.GetSpeedModifierAt(m_player.GetPosition());

	Vector2f movement = m_player.CalculateMovement(deltaTime, movementDirection, speedModifier);

	Rect collisionRect = m_player.GetCollisionRect();

	movement = m_world.ResolveMovement(collisionRect, movement);
	m_player.MovePlayer(movement);
}

void Application::Render()
{
	m_renderer.Clear();

	m_camera.SetPosition(m_player.GetPosition());

	const Vector2f cameraOrigin = GetCameraOrigin(
		m_camera.GetPosition(),
		{ m_world.GetTileWidth(), m_world.GetTileHeight() },
		m_window.GetCenter()
	);

	m_world.Render(m_renderer, cameraOrigin);

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
		const Vector2f screenPosition = WorldToScreen(entity->GetPosition(), { m_world.GetTileWidth(), m_world.GetTileHeight() }, cameraOrigin);

		entity->Render(m_renderer, screenPosition);
	}
	m_renderer.Present();
}