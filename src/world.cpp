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
	return true;
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