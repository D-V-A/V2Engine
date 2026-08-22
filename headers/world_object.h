#pragma once

#include "entity.h"

class WorldObject: public Entity
{
public:
	WorldObject();

public:
	Rect GetCollisionRect() const;

	Rect GetRenderOrderBounds() const;

private:
	Rect m_collisionRect{
	{-1.0f, -1.0f},//relative to map position
	{1.0f, 1.0f} };//collision size
};