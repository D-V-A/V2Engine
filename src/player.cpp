#include "player.h"

Player::Player() : Entity({ 1.0f, 1.0f }, { 0.5f, 1.0f })
{	
}

Vector2f Player::CalculateMovement(float deltaTime, const Vector2i& direction) const
{
	const float speed = (direction.x != 0 && direction.y != 0) ? 1.4142136f : 2.0f;

	return { speed * direction.x * deltaTime, -speed * direction.y * deltaTime };
}

Rect Player::GetCollisionRectAt(const Vector2f& pos) const
{
	Rect result = m_collisionRect;
	result.position.x += pos.x;
	result.position.y += pos.y;

	return result;
}

Rect Player::GetCollisionRect() const
{
	return GetCollisionRectAt(GetPosition());
}

void Player::MovePlayer(const Vector2f& movement)
{
	m_position.x += movement.x;
	m_position.y += movement.y;
}

Rect Player::GetRenderOrderBounds() const
{
	return GetCollisionRect();//temporal solution
}
