#include "player.h"
#include "renderer.h"
#include "isometric.h"

Vector2f Player::CalculateMovement(float deltaTime, const Vector2i& direction) const
{
	const float speed = (direction.x != 0 && direction.y != 0) ? 1.4142136f : 2.0f;

	return { speed * direction.x * deltaTime, -speed * direction.y * deltaTime };
}

void Player::Render(Renderer& renderer,	const Vector2f& screenPosition) const
{
	Rect renderRect = m_textureRectangle;

	renderRect.position = GetTopLeft(screenPosition, renderRect.size, m_pivot);

	renderer.DrawTexture(m_texture, renderRect);
}

bool Player::Initialize(Renderer& renderer)
{
	if(!m_texture.Load(renderer,"assets/txt/player.bmp"))
		return false;

	CalculateRectSize();
	return true;
}

void Player::CalculateRectSize()
{
	m_textureRectangle.size.x = static_cast<float>(m_texture.GetWidth());
	m_textureRectangle.size.y = static_cast<float>(m_texture.GetHeight());
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