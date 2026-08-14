#pragma once

#include "entity.h"

#include "types/vector2f.h"
#include "types/rect.h"

class Renderer;

class WorldObject: public Entity
{
public:
	WorldObject();

public:
	Rect GetCollisionRect() const;

private:
	Rect m_collisionRect{
	{-1.0f, -1.0f},//relative to map position
	{1.0f, 1.0f} };//collision size
};