#pragma once

#include <array>

#include "entity.h"

enum class PlayerState
{
	Idle,
	Walking,
	Count
	// TODO: add Running
};

enum class Direction
{
	East,
	SouthEast,
	South,
	SouthWest,
	West,
	NorthWest,
	North,
	NorthEast,
	Count
};

class Vector2i;

class Player: public Entity
{
public:
	Player();

public:	
	bool Initialize(Renderer& renderer);

	Vector2f CalculateMovement(float deltaTime, const Vector2i& direction, const float modifier) const;

	void MovePlayer(const Vector2f& movement);

	void UpdateAnimation(float deltaTime);

	void SetViewDirection(Direction viewDirection) { m_viewDirection = viewDirection; };
	void SetViewDirection(const Vector2f& direction);
	void SetState(PlayerState state);

	Rect GetCollisionRectAt(const Vector2f& position) const;
	Rect GetCollisionRect() const;

	void Render(Renderer& renderer, const Vector2f& screenPosition) const;
	const Texture& GetCurrentTexture(Rect& frame) const;

private:

	Rect m_collisionRect{
		{ -0.10f, -0.10f },//top left corner, relative to map position
		{0.2f, 0.4f} };//collision size

	Vector2f m_visualAnchor{ 67.0f, 84.0f };//center of mass relative to top left corner

	PlayerState m_state = PlayerState::Idle;
	Direction m_viewDirection = Direction::South;

	float m_walkDistance = 0.0f;
	float m_animationTime = 0.0f;
	int m_currentFrame = 0;

	std::array<Texture, static_cast<size_t>(PlayerState::Count)> m_textures;
};