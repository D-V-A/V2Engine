#include <cmath>
#include <filesystem>
#include <numbers>

#include "player.h"
#include "assets.h"
#include "renderer.h"
#include "isometric.h"

#include "types/vector2i.h"

Player::Player() : Entity({ 1.0f, 1.0f }, { 0.5f, 1.0f })
{	
	m_renderOrderBounds = m_collisionRect;
}

bool Player::Initialize(Renderer& renderer)
{
	for(int statesCount = 0; statesCount < static_cast<int>(PlayerState::Count); statesCount++)
	{
		for (int directionsCount = 0; directionsCount < static_cast<int>(Direction::Count); directionsCount++)
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

const Texture& Player::GetCurrentTexture(Rect& frame) const
{ 

	Rect source{
	{ /*frame_num*/1 * 128.0f, static_cast<int>(m_viewDirection) * 128.0f },
	{ 128.0f, 128.0f }
	};

	frame = source;

	return m_textures[static_cast<size_t>(m_state)]; 
};


void Player::Render(Renderer& renderer, const Vector2f& screenPosition) const
{
	Rect sourceFrame;

	const Texture& texture = GetCurrentTexture(sourceFrame);
	
	Rect renderRect;
	renderRect.size = { 128.0f,128.0f };

	//Rect renderRect;
	//renderRect.size = {
	//	static_cast<float>(texture.GetWidth()),
	//	static_cast<float>(texture.GetHeight())
	//};

	renderRect.position = GetTopLeft(screenPosition, renderRect.size, m_pivot);

	renderer.DrawTexture(texture, sourceFrame, renderRect);
}