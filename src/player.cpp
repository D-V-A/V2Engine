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
	renderer.DrawTexture(texture, rectangle);
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

bool Player::Initialize(Renderer& renderer)
{
	if(!texture.Load(renderer,"assets/txt/player.bmp"))
		return false;

	CalculateRect();
	return true;
}

void Player::CalculateRect()
{
	rectangle.size.x = static_cast<float>(texture.GetWidth());
	rectangle.size.y = static_cast<float>(texture.GetHeight());

	rectangle.position.x =
		(1280 - rectangle.size.x) / 2.0f;

	rectangle.position.y =
		(720 - rectangle.size.y) / 2.0f;
}