#pragma once

#include "entity.h"

class WorldObject: public Entity
{
public:
	WorldObject(Vector2f pos, Vector2f collision_size);

public:
	Rect GetCollisionRect() const;

	Rect GetRenderOrderBounds() const;

private:
	Rect m_collisionRect{
	{-1.0f, -1.0f},//relative to map position
	{1.0f, 1.0f} };//collision size
};