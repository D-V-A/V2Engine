#include <cassert>
#include <cmath>

#include "isometric.h"
#include "renderer.h"
#include "player.h"

#include "types/vector2i.h"

Player::Player() : Entity({ 1.0f, 1.0f }, { 0.5f, 1.0f })
{	
	m_renderOrderBounds = m_collisionRect;
}

bool Player::Initialize(Renderer& renderer)
{	
	return m_animator.InitializeTextures(renderer, "player");
}

float Player::GetViewDirectionPenalty() const
{
	const int difference = GetDirectionDifference(m_moveDirection, m_viewDirection);

	switch (difference)
	{
	case 0:
		return 1.0f;

	case 1:
	case 7:
		return 1.1f;

	case 2:
	case 6:
		return 1.2f;

	case 3:
	case 5:
		return 1.35f;

	case 4:
		return 1.5f;

	default:
		assert(false);
		return 1.0f;
	}
}

float Player::GetStateSpeedModifier() const
{
	switch (m_state)
	{
	case CharacterState::Idle:
		return 1.0f;
	case CharacterState::Walking:
		return 1.0f;
	case CharacterState::Running:
		return 1.6f;
	default:
		assert(false);
		return 0.0f;
	}
}

Vector2f Player::CalculateMovement(float deltaTime, const Vector2i& direction, const float surfaceTypeModifier) const
{
	const float speed = 
		(((direction.x != 0 && direction.y != 0) ? 1.4142136f : 2.0f) 
		* surfaceTypeModifier
		* GetStateSpeedModifier())
		/ GetViewDirectionPenalty();

	return { speed * direction.x * deltaTime, -speed * direction.y * deltaTime };
}

Rect Player::GetCollisionRectAt(const Vector2f& pos) const
{
	Rect result = m_collisionRect;
	result.x() += pos.x;
	result.y() += pos.y;

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

	m_animator.AddMovement(std::sqrt(movement.x * movement.x + movement.y * movement.y));
}

void Player::SetState(CharacterState state)
{
	if (m_state == state)
		return;

	const bool motionToMotion = (m_state != CharacterState::Idle && state != CharacterState::Idle);

	m_state = state;

	m_animator.ResetAnimation(!motionToMotion);
}

void Player::UpdateAnimation(float deltaTime)
{
	m_animator.AddTime(deltaTime);
	m_animator.SelectAnimation(m_state, m_moveDirection ,m_viewDirection);
	m_animator.UpdateAnimation();
}

void Player::Render(Renderer& renderer, const Vector2f& screenPosition) const
{
	const Texture& texture = m_animator.GetCurrentTexture();
	const Rect sourceFrame = m_animator.GetCurrentFrame(m_viewDirection);
	const float frameSize = m_animator.GetFrameSize();

	const Rect renderRect{
		{screenPosition.x - m_feetPositionInFrame.x,	screenPosition.y - m_feetPositionInFrame.y},
		{ frameSize,frameSize } 
	};

	renderer.DrawTexture(texture, sourceFrame, renderRect);
}
