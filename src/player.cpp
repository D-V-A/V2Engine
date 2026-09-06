#include <filesystem>
#include <numbers>
#include <cassert>
#include <cmath>

#include "renderer.h"
#include "player.h"
#include "assets.h"

#include "types/vector2i.h"

Player::Player() : Entity({ 1.0f, 1.0f }, { 0.5f, 1.0f })
{	
	m_renderOrderBounds = m_collisionRect;
}

bool Player::Initialize(Renderer& renderer)
{
	for(int statesCount = 0; statesCount < static_cast<int>(PlayerState::Count); statesCount++)
	{
		std::string strPath = "txt/player/";
		switch (static_cast<PlayerState>(statesCount))
		{
		case (PlayerState::Idle):
			strPath += "Idle.png";
			break;
		case (PlayerState::Walking):
			strPath += "Walk.png";
			break;
		default:
			return false;
		}
		
		std::filesystem::path assetPath = GetAssetPath(strPath);

		if (!InitializeTexture(renderer, m_textures[statesCount], assetPath.string().c_str()))
			return false;
	}
	return true;
}

Vector2f Player::CalculateMovement(float deltaTime, const Vector2i& direction, const float modifier) const
{
	const float speed = ((direction.x != 0 && direction.y != 0) ? 1.4142136f : 2.0f) * modifier;

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

	if (m_state == PlayerState::Walking)
	{
		m_walkDistance += std::sqrt(movement.x * movement.x + movement.y * movement.y);
	}
}

void Player::UpdateAnimation(float deltaTime)
{
	constexpr float frameDuration = 0.08f;
	constexpr float distancePerFrame = 0.14142136f;

	switch (m_state)
	{
	case PlayerState::Idle:
	
		m_animationTime += deltaTime;

		while (m_animationTime >= frameDuration)
		{
			m_animationTime -= frameDuration;
			m_currentFrame = (m_currentFrame + 1) % frameCount;
		}
		return;
	case PlayerState::Walking:
		while (m_walkDistance >= distancePerFrame)
		{
			m_walkDistance -= distancePerFrame;
			m_currentFrame = (m_currentFrame + 1) % frameCount;
		}
		return;
	default:
		assert(false);
		return;
	}	
}

void Player::SetState(PlayerState state)
{
	if (m_state == state)
		return;

	m_state = state;

	m_walkDistance = 0.0f;
	m_animationTime = 0.0f;
	m_currentFrame = 0;
}

/*				  N
			(-112.5; -67.5)

			NW			  NE
	(-157.5;-112.5) (-67.5; -22.5)

	   W						 E
(-157.5;-180)(180;157.5)  (- 22.5;22.5)

		 SW					SE
	(112.5;157.5)		(22.5;67.5)

				  S
			 (67.5;112.5)*/
void Player::SetViewDirection(const Vector2f& viewVector)
{
	if (viewVector.x == 0.0f && viewVector.y == 0.0f)
		return;

	constexpr Direction directions[] = {
	Direction::East,
	Direction::SouthEast,
	Direction::South,
	Direction::SouthWest,
	Direction::West,
	Direction::NorthWest,
	Direction::North,
	Direction::NorthEast
	};

	//that's some brainblowing algorythm
	const float angle = std::atan2(viewVector.y, viewVector.x);//radians [-Pi;Pi]
	float angleDegrees = angle * 180.0f / std::numbers::pi_v<float> ;//degreed [-180;180]
	if (angleDegrees < 0.0f)
		angleDegrees += 360.0f;

	SetViewDirection(directions[static_cast<int>((angleDegrees + 22.5f) / 45.0f) % 8]);
	/* Shift the angle by half a sector, divide the circle into 45-degree sectors, take the sector index, and wrap sector 8 back to sector 0.*/
}

const Texture& Player::GetCurrentTexture() const
{ 
	return m_textures[static_cast<size_t>(m_state)]; 
};

Rect Player::GetCurrentFrame() const
{
	Rect source{
		{ m_currentFrame * frameSize, static_cast<int>(m_viewDirection) * frameSize },//[frame,direction]
		{ frameSize, frameSize } 
	};

	return source;
}

void Player::Render(Renderer& renderer, const Vector2f& screenPosition) const
{
	const Texture& texture = GetCurrentTexture();
	const Rect sourceFrame = GetCurrentFrame();

	const Rect renderRect{
		{screenPosition.x - m_feetPositionInFrame.x,	screenPosition.y - m_feetPositionInFrame.y},
		{ frameSize,frameSize } 
	};

	renderer.DrawTexture(texture, sourceFrame, renderRect);
}