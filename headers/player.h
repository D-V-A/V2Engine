#pragma once

#include <array>

#include "entity.h"
#include "character_animator.h"

#include "types/character_info.h"


class Vector2i;

class Player: public Entity
{
public:
	Player();

public:	
	bool Initialize(Renderer& renderer);

	Vector2f CalculateMovement(float deltaTime, const Vector2i& direction, const float surfaceTypeModifier) const;

	void MovePlayer(const Vector2f& movement);

	void UpdateAnimation(float deltaTime);

	void SetViewDirection(Direction viewDirection) { m_viewDirection = viewDirection; };
	void SetMoveDirection(Direction viewDirection) { m_moveDirection = viewDirection; };
	void SetState(CharacterState state);

	Rect GetCollisionRectAt(const Vector2f& position) const;
	Rect GetCollisionRect() const;

	void Render(Renderer& renderer, const Vector2f& screenPosition) const override;

private:
	CharacterAnimator m_animator;

	float GetStateSpeedModifier() const;
	float GetViewDirectionPenalty() const;

	Rect m_collisionRect{
		{ -0.10f, -0.10f },//top left corner, relative to map position
		{0.2f, 0.4f} };//collision size

	Vector2f m_feetPositionInFrame{ 67.0f, 84.0f };//relative to the top-left corner of the frame

	CharacterState m_state = CharacterState::Idle;
	Direction m_viewDirection = Direction::South;
	Direction m_moveDirection = Direction::South;
};
