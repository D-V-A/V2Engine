#include "entity.h"
#include "isometric.h"
#include "renderer.h"

Entity::Entity(Vector2f pos) : m_position(pos)
{
}

Entity::Entity(Vector2f pos, Vector2f pivot): m_position(pos), m_pivot(pivot)
{
}

bool Entity::Initialize(Renderer& renderer, const char* texturePath)
{
	if (!m_texture.Load(renderer, texturePath))
		return false;

	m_textureRectangle.size.x = static_cast<float>(m_texture.GetWidth());
	m_textureRectangle.size.y = static_cast<float>(m_texture.GetHeight());

	return true;
}

void Entity::Render(Renderer& renderer, const Vector2f& screenPosition) const
{
	Rect renderRect = m_textureRectangle;

	renderRect.position = GetTopLeft(screenPosition, renderRect.size, m_pivot);

	renderer.DrawTexture(m_texture, renderRect);
}