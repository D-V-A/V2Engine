#include <iostream>
#include <vector>

#include "world/render_order.h"
#include "world/isometric.h"

#include "core/application.h"
#include "core/assets.h"

bool Application::Initialize()
{
	if (!m_sdl.Initialize())
	{
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << '\n';
		return false;
	}

	if (!m_window.Initialize("V2Engine", 1280, 720))
	{
		std::cerr << "Failed to create window: " << SDL_GetError() << '\n';

		return false;
	}
	
	if (!m_renderer.Initialize(m_window))
	{
		std::cerr << "Failed to create renderer: " << SDL_GetError() << '\n';

		return false;
	}

	if (!m_ttf.Initialize())
	{
		std::cerr << "Failed to create font context: " << SDL_GetError() << '\n';

		return false;
	}

	if (!m_textRenderer.Initialize(m_renderer))
	{
		std::cerr << "Failed to create text renderer: " << SDL_GetError() << '\n';

		return false;
	}

	std::filesystem::path assetPath = GetAssetPath("fnt/PTSans-Regular.ttf");
	if (!m_fonts.Load(assetPath, 24.0f))
	{
		std::cerr << "Failed to init font: " << SDL_GetError() << '\n';

		return false;
	}

	if (!m_interactionPopup.Initialize(m_textRenderer, m_fonts))
	{
		std::cerr << "Failed to create text panel: " << SDL_GetError() << '\n';

		return false;
	}

	if (!m_interactionPopup.SetText("Работаем, пацаны!"))
	{
		std::cerr << "Failed to set text panel text: " << SDL_GetError() << '\n';

		return false;
	}
	m_interactionPopup.SetPosition({ 15.0f, 15.0f });
	m_interactionPopup.SetTextColor(GetColor(Colors::White));
	m_interactionPopup.SetBackgroundColor(Color{0,0,0,50});
	m_interactionPopup.SetVisible(false);

	assetPath = GetAssetPath("map/map.json");
	InitializationResults worldInitRes = m_world.Initialize(m_renderer, assetPath.string().c_str());
	if (worldInitRes != InitializationResults::Success)
	{
		switch (worldInitRes)
		{
		case(InitializationResults::InfoLoadFail):
			std::cerr << "Failed to load map info: " << assetPath << '\n';
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

	if (!m_player.Initialize(m_renderer))
	{
		std::cerr << "Failed to init player model: " << SDL_GetError() << '\n';

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

	MovePlayer(deltaTime);

	m_camera.SetPosition(m_player.GetPosition());

	UpdatePlayerViewDirection();

	FindInteraction(m_player.GetPosition());

	m_player.UpdateAnimation(deltaTime);
}

void Application::FindInteraction(const Vector2f& position)
{
	const WorldObject* interactionTarget = m_world.FindInteractionTarget(m_player.GetCollisionRect());

	if (interactionTarget)
	{
		m_interactionPopup.SetText("[E] " + interactionTarget->GetInteractionText().value());
		m_interactionPopup.SetVisible(true);

		const Rect bounds = interactionTarget->GetRenderOrderBounds();

		const Vector2f popupWorldPosition{
			bounds.x() + bounds.width() * 0.5f,
			bounds.y() + bounds.height() * 0.5f
		};

		const Vector2f tileSize{ m_world.GetTileWidth(), m_world.GetTileHeight() };
		const Vector2f cameraOrigin = GetCameraOrigin(m_camera.GetPosition(), tileSize, m_window.GetCenter());

		Vector2f popupScreenPosition = WorldToScreen(popupWorldPosition, tileSize, cameraOrigin);

		const Vector2f panelSize = m_interactionPopup.GetSize();

		popupScreenPosition.x -= panelSize.x * 0.5f;
		popupScreenPosition.y -= panelSize.y + 10.0f;

		m_interactionPopup.SetPosition(popupScreenPosition);
	}
	else
	{
		m_interactionPopup.SetVisible(false);
	}
}

void Application::MovePlayer(float deltaTime)
{
	const Vector2i movementDirection = m_input.GetDirection();
	const bool running = m_input.GetShiftState();

	if (movementDirection.x == 0 && movementDirection.y == 0)
	{
		m_player.SetState(CharacterState::Idle);
		return;
	}

	//Player's movement
	const float speedModifier = m_world.GetSpeedModifierAt(m_player.GetPosition());
	Vector2f movement = m_player.CalculateMovement(deltaTime, movementDirection, speedModifier);
	Rect collisionRect = m_player.GetCollisionRect();
	movement = m_world.ResolveMovement(collisionRect, movement);

	m_player.SetState((movement.x == 0.0f && movement.y == 0.0f) ? CharacterState::Idle : !running ? CharacterState::Walking : CharacterState::Running);

	const Vector2f screenMovement = WorldVectorToScreen(movement, { m_world.GetTileWidth(), m_world.GetTileHeight() });
	m_player.SetMoveDirection(GetDirectionFromVector(screenMovement));

	m_player.MovePlayer(movement);
}

void Application::UpdatePlayerViewDirection()
{
	//Player's look direction
	const Vector2f mousePosition = m_input.GetMousePosition();
	const Vector2f cameraOrigin = GetCameraOrigin(m_camera.GetPosition(), { m_world.GetTileWidth(), m_world.GetTileHeight() }, m_window.GetCenter());
	const Vector2f playerScreenPosition = WorldToScreen(m_player.GetPosition(), { m_world.GetTileWidth(), m_world.GetTileHeight() }, cameraOrigin);
	const Vector2f sightVector{ mousePosition.x - playerScreenPosition.x, mousePosition.y - playerScreenPosition.y };
	m_player.SetViewDirection(GetDirectionFromVector(sightVector));
}

void Application::Render()
{
	m_renderer.Clear();
	const Vector2f cameraOrigin = GetCameraOrigin(m_camera.GetPosition(), { m_world.GetTileWidth(), m_world.GetTileHeight() }, m_window.GetCenter());

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

	m_interactionPopup.Render(m_renderer, m_textRenderer);

	m_renderer.Present();
}