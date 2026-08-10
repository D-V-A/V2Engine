#include "world.h"
#include "renderer.h"

#include "types/rect.h"

bool World::Initialize(Renderer& renderer)
{
	return floorTexture.Load(
		renderer,
		"assets/txt/grass_tile.bmp");
}

void World::Render(Renderer& renderer) const
{
	const float tileWidth =
		static_cast<float>(floorTexture.GetWidth());

	const float tileHeight =
		static_cast<float>(floorTexture.GetHeight());

	constexpr float windowWidth = 1280.0f;//todo: delete const
	constexpr float windowHeight = 720.0f;

	const float originX = windowWidth / 2;
	const float originY = (windowHeight - height * tileHeight) / 2;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			Rect tileRect;

			tileRect.size = {
				tileWidth,
				tileHeight
			};

			tileRect.position.x =
				originX +
				(x - y) * tileWidth / 2.0f;

			tileRect.position.y =
				originY +
				(x + y) * tileHeight / 2.0f;

			renderer.DrawTexture(
				floorTexture,
				tileRect);
		}
	}
}