#pragma once

#include "entity.h"

#include "types/vector2i.h"

class Player: public Entity
{
public:
	Player();

public:
	Vector2f CalculateMovement(float deltaTime, const Vector2i& direction) const;

	void MovePlayer(const Vector2f& movement);

	Rect GetCollisionRectAt(const Vector2f& position) const;
	Rect GetCollisionRect() const;

	Rect GetRenderOrderBounds() const;

private:

	Rect m_collisionRect{
		{ -0.25f, -0.25f },//top left corner, relative to map position
		{0.5f, 0.5f} };//collision size
};