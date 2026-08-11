#include "world_object.h"

#include "renderer.h"
#include "isometric.h"

bool WorldObject::Initialize(Renderer& renderer, const char* texturePath)
{
	if (!texture.Load(renderer, texturePath))// "assets/txt/player.bmp"))
		return false;

	CalculateRectSize();
	return true;
}

void WorldObject::CalculateRectSize()
{
	textureRectangle.size.x = static_cast<float>(texture.GetWidth());
	textureRectangle.size.y = static_cast<float>(texture.GetHeight());
}

void WorldObject::Render(Renderer& renderer, const Vector2f& screenPosition) const
{
	Rect renderRect = textureRectangle;

	renderRect.position = GetTopLeft(screenPosition, renderRect.size, pivot);

	renderer.DrawTexture(texture, renderRect);
}
