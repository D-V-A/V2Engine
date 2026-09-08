#include "entity.h"

Entity::Entity(Vector2f pos) : m_position(pos)
{
}

Entity::Entity(Vector2f pos, Vector2f renderFootprintSize) : m_position(pos)
{
	m_renderOrderBounds.position = { -renderFootprintSize.x, -renderFootprintSize.y };
	m_renderOrderBounds.size = renderFootprintSize;
}

Rect Entity::GetRenderOrderBounds() const
{
	Rect result = m_renderOrderBounds;

	result.x() += m_position.x;
	result.y() += m_position.y;

	return result;
}