#include <algorithm>
#include <stdlib.h>

#include "player.h"
#include "renderer.h"
#include "isometric.h"

void Player::MovePlayer(float deltaTime, Vector2i direction)
{
	const float speed =
		(direction.x != 0 && direction.y != 0)
		? 1.4142136f
		: 2.0f;

	position.x += speed * direction.x * deltaTime;
	position.y -= speed * direction.y * deltaTime;
}

void Player::Render(Renderer& renderer,	const Vector2f& screenPosition) const
{
	Rect renderRect = textureRectangle;

	renderRect.position = GetTopLeft(screenPosition, renderRect.size, pivot);

	renderer.DrawTexture(texture, renderRect);
}

void Player::CheckMapBorders(const Vector2f& worldSize)
{
	position.x = std::clamp(
		position.x,
		0.0f,
		static_cast<float>(worldSize.x));

	position.y = std::clamp(
		position.y,
		0.0f,
		static_cast<float>(worldSize.y));
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
	constexpr float windowWidth = 1280.0f;//todo: delete const
	constexpr float windowHeight = 720.0f;

	textureRectangle.size.x = static_cast<float>(texture.GetWidth());
	textureRectangle.size.y = static_cast<float>(texture.GetHeight());

	textureRectangle.position.x =
		(windowWidth - textureRectangle.size.x) / 2.0f;

	textureRectangle.position.y =
		(windowHeight - textureRectangle.size.y) / 2.0f;
}