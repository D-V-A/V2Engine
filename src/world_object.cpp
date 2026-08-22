#include "world_object.h"

WorldObject::WorldObject(Vector2f pos, Vector2f collision_size) : Entity(pos, {0.5f, 1.0f})
{
	m_collisionRect.size = collision_size;
}

Rect WorldObject::GetCollisionRect() const
{
	Rect result = m_collisionRect;
	result.position.x += m_position.x;
	result.position.y += m_position.y;

	return result;
}

Rect WorldObject::GetRenderOrderBounds() const
{
	return GetCollisionRect();//temporal solution
}