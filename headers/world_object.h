#pragma once

#include <optional>

#include "entity.h"

class WorldObject: public Entity
{
public:
	WorldObject(Vector2f position, Vector2f renderFootprintSize, std::optional<Rect> collisionRect);

public:
	Rect GetCollisionRect() const;

	bool HasCollision() const;

private:
	std::optional<Rect> m_collisionRect;
};