#include "player.h"
#include "renderer.h"
#include "isometric.h"

Vector2f Player::CalculatePlayerMovement(float deltaTime, Vector2i direction) const
{
	const float speed =
		(direction.x != 0 && direction.y != 0)
		? 1.4142136f
		: 2.0f;

	Vector2f newPosition = position;

	newPosition.x += speed * direction.x * deltaTime;
	newPosition.y -= speed * direction.y * deltaTime;

	return(newPosition);
}

void Player::Render(Renderer& renderer,	const Vector2f& screenPosition) const
{
	Rect renderRect = textureRectangle;

	renderRect.position = GetTopLeft(screenPosition, renderRect.size, pivot);

	renderer.DrawTexture(texture, renderRect);
}

bool Player::Initialize(Renderer& renderer)
{
	if(!texture.Load(renderer,"assets/txt/player.bmp"))
		return false;

	CalculateRectSize();
	return true;
}

void Player::CalculateRectSize()
{
	textureRectangle.size.x = static_cast<float>(texture.GetWidth());
	textureRectangle.size.y = static_cast<float>(texture.GetHeight());
}

Rect Player::GetCollisionRectAt(const Vector2f& pos) const
{
	Rect result = collisionRect;
	result.position.x += pos.x;
	result.position.y += pos.y;

	return result;
}