#include <cassert>
#include <cmath>

#include "world/isometric.h"

#include "graphics/renderer.h"

#include "entities/character.h"

#include "types/vector2i.h"

Character::Character() : Entity({ 1.0f, 1.0f }, { 0.5f, 1.0f })
{	
	m_renderOrderBounds = m_collisionRect;
}

bool Character::Initialize(ResourceManager& resManager, std::string charName)
{	
	return m_animator.InitializeTextures(resManager, charName);
}

float Character::GetViewDirectionPenalty() const
{
	const int difference = GetDirectionDifference(m_state.moveDirection, m_state.viewDirection);

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

float Character::GetStateSpeedModifier() const
{
	switch (m_state.movement)
	{
	case CharacterMovement::Idle:
		return 1.0f;
	case CharacterMovement::Walking:
		return 1.0f;
	case CharacterMovement::Running:
		return 1.6f;
	default:
		assert(false);
		return 0.0f;
	}
}

Vector2f Character::CalculateMovement(float deltaTime, const Vector2i& direction, const float surfaceTypeModifier) const
{
	const float speed = 
		(((direction.x != 0 && direction.y != 0) ? 1.4142136f : 2.0f) 
		* surfaceTypeModifier
		* GetStateSpeedModifier())
		/ GetViewDirectionPenalty();

	return { speed * direction.x * deltaTime, -speed * direction.y * deltaTime };
}

Rect Character::GetCollisionRectAt(const Vector2f& pos) const
{
	Rect result = m_collisionRect;
	result.x() += pos.x;
	result.y() += pos.y;

	return result;
}

Rect Character::GetCollisionRect() const
{
	return GetCollisionRectAt(GetPosition());
}

void Character::MoveCharacter(const Vector2f& movement)
{
	m_position.x += movement.x;
	m_position.y += movement.y;

	m_animator.AddMovement(std::sqrt(movement.x * movement.x + movement.y * movement.y));
}

void Character::SetMovement(CharacterMovement move)
{
	if (m_state.movement == move)
		return;

	const bool motionToMotion = (m_state.movement != CharacterMovement::Idle && move != CharacterMovement::Idle);

	m_state.movement = move;

	m_animator.ResetAnimation(!motionToMotion);
}

void Character::UpdateAnimation(float deltaTime)
{
	m_animator.AddTime(deltaTime);
	m_animator.SelectAnimation(m_state, m_state.moveDirection , m_state.viewDirection);
	m_animator.UpdateAnimation();
}

void Character::Render(Renderer& renderer, const Vector2f& screenPosition) const
{
	const Texture& texture = m_animator.GetCurrentTexture();
	const Rect sourceFrame = m_animator.GetCurrentFrame(m_state.viewDirection);
	const float frameSize = m_animator.GetFrameSize();

	const Rect renderRect{
		{screenPosition.x - m_feetPositionInFrame.x,	screenPosition.y - m_feetPositionInFrame.y},
		{ frameSize,frameSize } 
	};

	renderer.DrawTexture(texture, sourceFrame, renderRect);
}
