#include <algorithm>

#include "world.h"
#include "renderer.h"
#include "isometric.h"

#include "types/rect.h"

bool World::Initialize(Renderer& renderer)
{
	if (!floorTexture.Load(renderer, "assets/txt/grass_tile.bmp"))
		return false;

	tileWidth = static_cast<float>(floorTexture.GetWidth());

	tileHeight = static_cast<float>(floorTexture.GetHeight());

	constexpr float windowWidth = 1280.0f;//todo: delete const
	constexpr float windowHeight = 720.0f;

	origin.x = windowWidth / 2;
	origin.y = (windowHeight - height * tileHeight) / 2;

	CreateObjects();

	return true;
}

void World::CreateObjects() 
{
	objects.resize(1);
}

bool World::InitializeObjects(Renderer& renderer, const char* texturePath)
{
	for (WorldObject& object : objects)
	{
		if (!object.Initialize(renderer, texturePath))
		{
			return false;
		}
	}
	return true;
}

void World::RenderObjects(Renderer& renderer) const
{
	for (const WorldObject& object : objects)
	{
		Vector2f screenPosition = WorldToScreen(object.GetPosition(), { GetTileWidth(), GetTileHeight() }, GetOrigin());
		object.Render(renderer, screenPosition);
	}
}

void World::Render(Renderer& renderer) const
{

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			Rect tileRect;

			tileRect.size = { tileWidth, tileHeight };

			const Vector2f tileCenter =	WorldToScreen( { static_cast<float>(x), static_cast<float>(y) }, tileRect.size, origin);

			tileRect.position = GetTopLeft(tileCenter, tileRect.size, pivot);

			renderer.DrawTexture(
				floorTexture,
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

float World::ResolveMovementX(const Rect& collisionRect, const float& movement) const
{
	float allowedMovement = movement;

	if (movement > 0.0f)
	{
		const float right = collisionRect.position.x + collisionRect.size.x;

		const float worldRight = static_cast<float>(width);
		const float playerRight = collisionRect.position.x + collisionRect.size.x;
		allowedMovement = std::min(allowedMovement, worldRight - playerRight);

		for (const WorldObject& object : objects)
		{
			const Rect& objectRect = object.GetCollisionRect();

			const bool overlapsY =
				collisionRect.position.y < objectRect.position.y + objectRect.size.y &&
				collisionRect.position.y + collisionRect.size.y > objectRect.position.y;

			if (!overlapsY)
				continue;

			if (right <= objectRect.position.x)
			{
				const float distance = objectRect.position.x - right;

				if (distance < allowedMovement)
					allowedMovement = distance;
			}
		}
	}
	else if (movement < 0.0f)
	{
		const float left = collisionRect.position.x;

		const float playerLeft = collisionRect.position.x;
		allowedMovement = -std::min(abs(allowedMovement), playerLeft);//worldLeft == 0

		for (const WorldObject& object : objects)
		{
			const Rect& objectRect = object.GetCollisionRect();

			const bool overlapsY =
				collisionRect.position.y < objectRect.position.y + objectRect.size.y &&
				collisionRect.position.y + collisionRect.size.y > objectRect.position.y;

			if (!overlapsY)
				continue;

			if (left >= objectRect.position.x + objectRect.size.x)
			{
				const float distance = left - (objectRect.position.x + objectRect.size.x) ;

				if (distance < abs(allowedMovement))
					allowedMovement = -distance;
			}
		}
	}

	return allowedMovement;
}

float World::ResolveMovementY(const Rect& collisionRect, const float& movement) const
{
	float allowedMovement = movement;

	if (movement > 0.0f)
	{
		const float bottom = collisionRect.position.y + collisionRect.size.y;

		const float worldBottom = static_cast<float>(height);
		const float playerBottom = collisionRect.position.y + collisionRect.size.y;
		allowedMovement = std::min(allowedMovement, worldBottom - playerBottom);

		for (const WorldObject& object : objects)
		{
			const Rect& objectRect = object.GetCollisionRect();

			const bool overlapsX =
				collisionRect.position.x < objectRect.position.x + objectRect.size.x &&
				collisionRect.position.x + collisionRect.size.x > objectRect.position.x;

			if (!overlapsX)
				continue;

			if (bottom <= objectRect.position.y)
			{
				const float distance = objectRect.position.y - bottom;

				if (distance < allowedMovement)
					allowedMovement = distance;
			}
		}
	}
	else if (movement < 0.0f)
	{
		const float top = collisionRect.position.y;

		const float playerTop = collisionRect.position.y;
		allowedMovement = - std::min(abs(allowedMovement), playerTop);//worldTop == 0

		for (const WorldObject& object : objects)
		{
			const Rect& objectRect = object.GetCollisionRect();

			const bool overlapsX =
				collisionRect.position.x < objectRect.position.x + objectRect.size.x &&
				collisionRect.position.x + collisionRect.size.x > objectRect.position.x;

			if (!overlapsX)
				continue;

			if (top >= objectRect.position.y + objectRect.size.y)
			{
				const float distance = top - (objectRect.position.y + objectRect.size.y);

				if (distance < abs(allowedMovement))
					allowedMovement = -distance;
			}
		}
	}

	return allowedMovement;
}