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
	North,
	NorthEast,
	East,
	SouthEast,
	South,
	SouthWest,
	West,
	NorthWest,
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

	void SetViewDirection(Direction viewDirection) { m_viewDirection = viewDirection; };
	void SetViewDirection(const Vector2f& direction);
	void SetState(PlayerState state) { m_state = state; };

	Rect GetCollisionRectAt(const Vector2f& position) const;
	Rect GetCollisionRect() const;
	const Texture& GetCurrentTexture() const override { return m_textures[static_cast<size_t>(m_state)][static_cast<size_t>(m_viewDirection)]; };

private:

	Rect m_collisionRect{
		{ -0.20f, -0.20f },//top left corner, relative to map position
		{0.4f, 0.4f} };//collision size

	PlayerState m_state = PlayerState::Idle;
	Direction m_viewDirection = Direction::South;

	//m_textures[states_count][directions_count]
	std::array<std::array<Texture, static_cast<size_t>(Direction::Count)>, static_cast<size_t>(PlayerState::Count)> m_textures;
};