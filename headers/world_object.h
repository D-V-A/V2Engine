#pragma once

#include <optional>

#include "entity.h"

class WorldObject: public Entity
{
public:
	WorldObject(Vector2f position, Vector2f renderFootprintSize, std::optional<Rect> collisionRect);

	bool Initialize(Renderer& renderer, const char* texturePath);

public:
	bool HasCollision() const;
	Rect GetCollisionRect() const;

private:
	std::optional<Rect> m_collisionRect;
};