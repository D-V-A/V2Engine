#include "world_object.h"

WorldObject::WorldObject(Vector2f pos, Vector2f renderFootprintSize) : Entity(pos, {0.5f, 1.0f})
{
	m_renderOrderBounds.position = { -renderFootprintSize.x, -renderFootprintSize.y };
	m_renderOrderBounds.size = renderFootprintSize;

	m_collisionRect = m_renderOrderBounds;
}

Rect WorldObject::GetCollisionRect() const
{
	Rect result = m_collisionRect;
	result.x() += m_position.x;
	result.y() += m_position.y;

	return result;
}