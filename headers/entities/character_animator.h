#pragma once
#include <string>
#include <array>

#include "graphics/texture.h"

#include "types/rect.h"
#include "types/character_info.h"

enum class CharacterAnimation
{
	Idle,
	WalkForward,
	StrafeLeft,
	StrafeRight,
	RunForward,
	MoveBackwards,
	Count
};


class CharacterAnimator
{
public:
	bool InitializeTextures(Renderer& renderer, std::string charName);

	void AddMovement(float dst) { m_moveDistance += dst; }
	void AddTime(float time)	{ m_animationTime += time; }

	void ResetAnimation(bool fullReset = true);

	void UpdateAnimation();
	void SelectAnimation(CharacterState state, const Direction movementDirection, const Direction viewDirection);

	const Texture& GetCurrentTexture() const;
	Rect GetCurrentFrame(Direction viewDirection) const;
	float GetFrameSize() const { return frameSize; }

private:
	CharacterAnimation m_animation = CharacterAnimation::Idle;
	float m_animationTime = 0.0f;
	float m_moveDistance = 0.0f;
	int m_currentFrame = 0;

	std::array<Texture, static_cast<size_t>(CharacterAnimation::Count)> m_textures;

	static constexpr float frameSize = 128.0f;
	static constexpr int frameCount = 14;
};