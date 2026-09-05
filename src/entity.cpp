#include "entity.h"
#include "isometric.h"
#include "renderer.h"

Entity::Entity(Vector2f pos) : m_position(pos)
{
}

Entity::Entity(Vector2f pos, Vector2f renderFootprintSize) : m_position(pos)
{
	m_renderOrderBounds.position = { -renderFootprintSize.x, -renderFootprintSize.y };
	m_renderOrderBounds.size = renderFootprintSize;
}

bool Entity::InitializeTexture(Renderer& renderer, Texture& texture, const char* texturePath)
{
	if (!texture.Load(renderer, texturePath))
		return false;

	return true;
}

void Entity::Render(Renderer& renderer, const Vector2f& screenPosition) const
{
	const Texture& texture = GetCurrentTexture();

	Rect renderRect;
	renderRect.size = {
		static_cast<float>(texture.GetWidth()),
		static_cast<float>(texture.GetHeight())
	};

	renderRect.position = GetTopLeft(screenPosition, renderRect.size, m_pivot);

	renderer.DrawTexture(texture, renderRect);
}

Rect Entity::GetRenderOrderBounds() const
{
	Rect result = m_renderOrderBounds;

	result.x() += m_position.x;
	result.y() += m_position.y;

	return result;
}