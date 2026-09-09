#include <filesystem>
#include <cassert>

#include "entities/character_animator.h"

#include "world/isometric.h"

#include "graphics/renderer.h"

#include "core/assets.h"


bool CharacterAnimator::InitializeTextures(Renderer& renderer, std::string charName)
{
	for (int animationsCount = 0; animationsCount < static_cast<int>(CharacterAnimation::Count); animationsCount++)
	{		
		std::string strPath = "txt/" + charName + "/";
		switch (static_cast<CharacterAnimation>(animationsCount))
		{
		case (CharacterAnimation::Idle):
			strPath += "Idle.png";
			break;
		case (CharacterAnimation::WalkForward):
			strPath += "Walk.png";
			break;
		case (CharacterAnimation::RunForward):
			strPath += "Run.png";
			break;
		case (CharacterAnimation::StrafeLeft):
			strPath += "StrafeLeft.png";
			break;
		case (CharacterAnimation::StrafeRight):
			strPath += "StrafeRight.png";
			break;
		case (CharacterAnimation::MoveBackwards):
			strPath += "MoveBackwards.png";
			break;
		default:
			return false;
		}

		std::filesystem::path assetPath = GetAssetPath(strPath);

		if (!m_textures[animationsCount].Load(renderer, assetPath.string().c_str()))
			return false;
	}
	return true;
}

void CharacterAnimator::ResetAnimation(bool fullReset)
{
	m_animationTime = 0.0f;
	m_moveDistance = 0.0f;

	if (fullReset)
		m_currentFrame = 0;
}

void CharacterAnimator::UpdateAnimation()
{
	constexpr float frameDuration = 0.08f;
	constexpr float moveDistancePerFrame = 0.14142136f;

	switch (m_animation)
	{
	case CharacterAnimation::Idle:

		while (m_animationTime >= frameDuration)
		{
			m_animationTime -= frameDuration;
			m_currentFrame = (m_currentFrame + 1) % frameCount;
		}
		return;
	case CharacterAnimation::WalkForward:
		while (m_moveDistance >= moveDistancePerFrame)
		{
			m_moveDistance -= moveDistancePerFrame;
			m_currentFrame = (m_currentFrame + 1) % frameCount;
		}
		return;
	case CharacterAnimation::RunForward:
		while (m_moveDistance >= moveDistancePerFrame)
		{
			m_moveDistance -= moveDistancePerFrame;
			m_currentFrame = (m_currentFrame + 1) % frameCount;
		}
		return;
	case CharacterAnimation::StrafeLeft:

		while (m_moveDistance >= moveDistancePerFrame)
		{
			m_moveDistance -= moveDistancePerFrame;
			m_currentFrame = (m_currentFrame + 1) % frameCount;
		}
		return;
	case CharacterAnimation::StrafeRight:
		while (m_moveDistance >= moveDistancePerFrame)
		{
			m_moveDistance -= moveDistancePerFrame;
			m_currentFrame = (m_currentFrame + 1) % frameCount;
		}
		return;
	case CharacterAnimation::MoveBackwards:
		while (m_moveDistance >= moveDistancePerFrame)
		{
			m_moveDistance -= moveDistancePerFrame;
			m_currentFrame = (m_currentFrame + 1) % frameCount;
		}
		return;
	default:
		assert(false);
		return;
	}
}

void CharacterAnimator::SelectAnimation(CharacterState state, Direction movementDirection, Direction viewDirection)
{
	const int difference = GetDirectionDifference(movementDirection, viewDirection);

	switch (state)
	{
	case(CharacterState::Idle):
		m_animation = CharacterAnimation::Idle;
		return;
	case(CharacterState::Running):
		if (difference == 0) 
		{
			m_animation = CharacterAnimation::RunForward;
			return;
		}
	case(CharacterState::Walking):
		switch (difference)
		{
		case 0:
			m_animation = CharacterAnimation::WalkForward;
			break;
		case 1:
		case 2: 
		case 3:
			m_animation = CharacterAnimation::StrafeLeft;
			break;
		case 4:
			m_animation = CharacterAnimation::MoveBackwards;
			break;
		case 5:
		case 6:
		case 7:
			m_animation = CharacterAnimation::StrafeRight;
			break;
		default:
			assert(false);
		}
		return;
	default:
		assert(false);
		m_animation = CharacterAnimation::Idle;
		return;
	}
}

const Texture& CharacterAnimator::GetCurrentTexture() const
{
	return m_textures[static_cast<size_t>(m_animation)];
};

Rect CharacterAnimator::GetCurrentFrame(Direction viewDirection) const
{
	Rect source{
		{ m_currentFrame * frameSize, static_cast<int>(viewDirection) * frameSize },//[frame,direction]
		{ frameSize, frameSize }
	};

	return source;
}