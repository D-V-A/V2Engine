#include <cmath>
#include <filesystem>
#include <numbers>

#include "player.h"
#include "assets.h"

#include "types/vector2i.h"

Player::Player() : Entity({ 1.0f, 1.0f }, { 0.5f, 1.0f })
{	
	m_renderOrderBounds = m_collisionRect;
}

bool Player::Initialize(Renderer& renderer)
{
	for(int state_count = 0; state_count < (int)PlayerState::Count; state_count++)
	{
		for (int dir_count = 0; dir_count < (int)Direction::Count; dir_count++)
		{
			std::string str_path = "txt/player/";
			switch ((PlayerState)state_count)
			{
			case (PlayerState::Idle):
				str_path += "idle/";
				break;
			case (PlayerState::Walking):
				str_path += "walk/";
				break;
			default:
				return false;
			}

			switch ((Direction)dir_count)
			{
			case (Direction::North):
				str_path += "N";
				break;
			case (Direction::NorthEast):
				str_path += "NE";
				break;
			case (Direction::East):
				str_path += "E";
				break;
			case (Direction::SouthEast):
				str_path += "SE";
				break;
			case (Direction::South):
				str_path += "S";
				break;
			case (Direction::SouthWest):
				str_path += "SW";
				break;
			case (Direction::West):
				str_path += "W";
				break;
			case (Direction::NorthWest):
				str_path += "NW";
				break;
			default:
				return false;
			}

			str_path += ".png";
			std::filesystem::path asset_path = GetAssetPath(str_path);

			if (!InitializeTexture(renderer, m_textures[state_count][dir_count], asset_path.string().c_str()))
				return false;
		}		
	}
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