#include <algorithm>
#include <cmath>

#include "world.h"
#include "renderer.h"
#include "isometric.h"

#include "types/rect.h"

bool World::Initialize(Renderer& renderer)
{
	if (!m_floorTexture.Load(renderer, "assets/txt/grass_tile.bmp"))
		return false;

	m_tileWidth = static_cast<float>(m_floorTexture.GetWidth());

	m_tileHeight = static_cast<float>(m_floorTexture.GetHeight());

	constexpr float windowWidth = 1280.0f;//todo: delete const
	constexpr float windowHeight = 720.0f;

	m_origin.x = windowWidth / 2;
	m_origin.y = (windowHeight - m_height * m_tileHeight) / 2;

	CreateObjects();

	return true;
}

void World::CreateObjects() 
{
	m_objects.resize(3);
	float i = 2.0f;
	for (auto it = m_objects.begin(); it != m_objects.end(); it++)
	{
		it->SetPosition({ i ,2 });
		i += 2.0f;
	}
}

bool World::InitializeObjects(Renderer& renderer, const char* texturePath)
{
	for (WorldObject& object : m_objects)
	{
		if (!object.Initialize(renderer, texturePath))
		{
			return false;
		}
	}
	return true;
}

void World::Render(Renderer& renderer) const
{

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			Rect tileRect;

			tileRect.size = { m_tileWidth, m_tileHeight };

			const Vector2f tileCenter =	WorldToScreen( { static_cast<float>(x), static_cast<float>(y) }, tileRect.size, m_origin);

			tileRect.position = GetTopLeft(tileCenter, tileRect.size, m_pivot);

			renderer.DrawTexture(
				m_floorTexture,
				tileRect);
		}
	}
}

Vector2f World::ResolveMovement(const Rect& collisionRect, const Vector2f& movement) const
{
	Vector2f result = movement;
	Rect movedRect = collisionRect;

	result.x = ResolveMovementX(collisionRect, movement.x);
	movedRect.position.x += result.x;

	result.y = ResolveMovementY(movedRect, movement.y);

	return result;
}

float World::ResolveMovementX(const Rect& collisionRect, float movement) const
{
	auto OverlapsY = [](const Rect& a, const Rect& b)
		{
			return a.position.y < b.position.y + b.size.y && a.position.y + a.size.y > b.position.y;
		};

	float allowedMovement = movement;

	if (movement > 0.0f)
	{
		const float worldRight = static_cast<float>(m_width);
		const float playerRight = collisionRect.position.x + collisionRect.size.x;
		allowedMovement = std::min(allowedMovement, worldRight - playerRight);

		for (const WorldObject& object : m_objects)
		{
			const Rect objectRect = object.GetCollisionRect();

			if (!OverlapsY(collisionRect, objectRect))
				continue;

			if (playerRight <= objectRect.position.x)
			{
				const float distance = objectRect.position.x - playerRight;

				if (distance < allowedMovement)
					allowedMovement = distance;
			}
		}
	}
	else if (movement < 0.0f)
	{
		const float playerLeft = collisionRect.position.x;
		allowedMovement = -std::min(std::abs(allowedMovement), playerLeft);//worldLeft == 0

		for (const WorldObject& object : m_objects)
		{
			const Rect objectRect = object.GetCollisionRect();

			if (!OverlapsY(collisionRect, objectRect))
				continue;

			if (playerLeft >= objectRect.position.x + objectRect.size.x)
			{
				const float distance = playerLeft - (objectRect.position.x + objectRect.size.x) ;

				if (distance < std::abs(allowedMovement))
					allowedMovement = -distance;
			}
		}
	}

	return allowedMovement;
}

float World::ResolveMovementY(const Rect& collisionRect, float movement) const
{
	auto OverlapsX = [](const Rect& a, const Rect& b)
		{
			return a.position.x < b.position.x + b.size.x && a.position.x + a.size.x > b.position.x;
		};

	float allowedMovement = movement;

	if (movement > 0.0f)
	{
		const float worldBottom = static_cast<float>(m_height);
		const float playerBottom = collisionRect.position.y + collisionRect.size.y;
		allowedMovement = std::min(allowedMovement, worldBottom - playerBottom);

		for (const WorldObject& object : m_objects)
		{
			const Rect objectRect = object.GetCollisionRect();

			if (!OverlapsX(collisionRect, objectRect))
				continue;

			if (playerBottom <= objectRect.position.y)
			{
				const float distance = objectRect.position.y - playerBottom;

				if (distance < allowedMovement)
					allowedMovement = distance;
			}
		}
	}
	else if (movement < 0.0f)
	{
		const float playerTop = collisionRect.position.y;
		allowedMovement = - std::min(std::abs(allowedMovement), playerTop);//worldTop == 0

		for (const WorldObject& object : m_objects)
		{
			const Rect objectRect = object.GetCollisionRect();

			if (!OverlapsX(collisionRect, objectRect))
				continue;

			if (playerTop >= objectRect.position.y + objectRect.size.y)
			{
				const float distance = playerTop - (objectRect.position.y + objectRect.size.y);

				if (distance < std::abs(allowedMovement))
					allowedMovement = -distance;
			}
		}
	}

	return allowedMovement;
}