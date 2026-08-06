#include <algorithm>
#include <stdlib.h>

#include "player.h"
#include "renderer.h"

void Player::MovePlayer(float deltaTime, Vector2i direction)
{
	const float speed = (direction.x != 0 && direction.y != 0) ? 141.42136f : 200.0f; // пикселей в секунду
	
	rectangle.position.x += speed * direction.x * deltaTime;
	rectangle.position.y -= speed * direction.y * deltaTime;
}

void Player::Render(Renderer& renderer) const
{
	renderer.FillRect(rectangle);
}

void Player::CheckScreenBorders(int width, int height)
{
	rectangle.position.x = std::clamp(
		rectangle.position.x,
		0.0f,
		width - rectangle.size.x);
	rectangle.position.y = std::clamp(
		rectangle.position.y,
		0.0f,
		height - rectangle.size.y);
}