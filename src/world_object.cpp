#include "world_object.h"

WorldObject::WorldObject() : Entity({ 4.0f, 5.0f }, { 0.5f, 1.0f })
{
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