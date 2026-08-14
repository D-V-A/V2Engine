#include <algorithm>

#include "world.h"
#include "renderer.h"
#include "isometric.h"
#include "collision.h"

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
		if (!object.Initialize(renderer, "assets/txt/crate.bmp"))
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

bool World::ValidatePlayerPos(Rect collisionRect)
{
	if (!CheckMapBorders(collisionRect))
	{
		return false;
	}

	for (const WorldObject& object : objects)
	{
		if (!IsOutsideBorders(collisionRect, object.GetCollisionRect()))
		{
			return false;
		}
	}

	return true;
}

bool World::CheckMapBorders(Rect collisionRect)
{
	Vector2f world_size = GetSize();

	if (!IsInsideBorders(collisionRect, { world_size.x/2, world_size.y/2, world_size }))
	{
		return false;
	}
	return true;
}
