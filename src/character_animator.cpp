#include <filesystem>
#include <cassert>

#include "character_animator.h"
#include "renderer.h"
#include "assets.h"


bool CharacterAnimator::InitializeTextures(Renderer& renderer, std::string charName)
{
	for (int statesCount = 0; statesCount < static_cast<int>(CharacterState::Count); statesCount++)
	{		
		std::string strPath = "txt/" + charName + "/";
		switch (static_cast<CharacterState>(statesCount))
		{
		case (CharacterState::Idle):
			strPath += "Idle.png";
			break;
		case (CharacterState::Walking):
			strPath += "Walk.png";
			break;
		case (CharacterState::Running):
			strPath += "Run.png";
			break;
		default:
			return false;
		}

		std::filesystem::path assetPath = GetAssetPath(strPath);

		if (!m_textures[statesCount].Load(renderer, assetPath.string().c_str()))
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

void CharacterAnimator::UpdateAnimation(CharacterAnimation state)
{
	constexpr float frameDuration = 0.08f;
	constexpr float moveDistancePerFrame = 0.14142136f;

	switch (state)
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
	default:
		assert(false);
		return;
	}
}

CharacterAnimation CharacterAnimator::SelectAnimation(CharacterState state, Direction movementDirection, Direction viewDirection)
{
	if (state == CharacterState::Idle)
		return CharacterAnimation::Idle;

	// compare movement direction with view direction

	if (state == CharacterState::Walking)
	{
		return CharacterAnimation::WalkForward;
	}

	if (state == CharacterState::Running)
	{
		return CharacterAnimation::RunForward;
	}
}

const Texture& CharacterAnimator::GetCurrentTexture(CharacterState state) const
{
	return m_textures[static_cast<size_t>(state)];
};

Rect CharacterAnimator::GetCurrentFrame(Direction viewDirection) const
{
	Rect source{
		{ m_currentFrame * frameSize, static_cast<int>(viewDirection) * frameSize },//[frame,direction]
		{ frameSize, frameSize }
	};

	return source;
}