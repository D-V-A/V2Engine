#include "world_object.h"

#include "renderer.h"
#include "isometric.h"

bool WorldObject::Initialize(Renderer& renderer, const char* texturePath)
{
	if (!m_texture.Load(renderer, texturePath))// "assets/txt/player.bmp"))
		return false;

	CalculateRectSize();
	return true;
}

void WorldObject::CalculateRectSize()
{
	m_textureRectangle.size.x = static_cast<float>(m_texture.GetWidth());
	m_textureRectangle.size.y = static_cast<float>(m_texture.GetHeight());
}

void WorldObject::Render(Renderer& renderer, const Vector2f& screenPosition) const
{
	Rect renderRect = m_textureRectangle;

	renderRect.position = GetTopLeft(screenPosition, renderRect.size, pivot);

	renderer.DrawTexture(m_texture, renderRect);
}

Rect WorldObject::GetCollisionRect() const
{
	Rect result = m_collisionRect;
	result.position.x += m_position.x;
	result.position.y += m_position.y;

	return result;
}