#include "world/isometric.h"

#include <numbers>
#include <cmath>

Vector2f WorldToScreen(const Vector2f& worldPosition, const Vector2f& size /*tile size*/, const Vector2f& origin)
{
	return{	origin.x + (worldPosition.x - worldPosition.y) * size.x / 2.0f,
			origin.y + (worldPosition.x + worldPosition.y) * size.y / 2.0f };
}

Vector2f WorldVectorToScreen(const Vector2f& worldVector, const Vector2f& tileSize)
{
	return {
		(worldVector.x - worldVector.y) * tileSize.x / 2.0f,
		(worldVector.x + worldVector.y) * tileSize.y / 2.0f
	};
}

Vector2f GetTopLeft(const Vector2f& anchor, const Vector2f& size, const Vector2f& pivot)
{
	return{	anchor.x - size.x * pivot.x,
			anchor.y - size.y * pivot.y };
}

bool IsBehind(const Rect& first, const Rect& second)
{
	const float firstRight = first.x() + first.width();
	const float firstBottom = first.y() + first.height();

	return firstRight <= second.x() ||
		firstBottom <= second.y();
}

Vector2f GetCameraOrigin(const Vector2f& cameraPosition, const Vector2f& tileSize, const Vector2f& screenCenter)
{
	const Vector2f projected = WorldToScreen(cameraPosition, tileSize, {});

	return {
		screenCenter.x - projected.x,
		screenCenter.y - projected.y
	};
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
Direction GetDirectionFromVector(const Vector2f& viewVector)
{
		if (viewVector.x == 0.0f && viewVector.y == 0.0f)
			return Direction::South;

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
		float angleDegrees = angle * 180.0f / std::numbers::pi_v<float>;//degreed [-180;180]
		if (angleDegrees < 0.0f)
			angleDegrees += 360.0f;

		return directions[static_cast<int>((angleDegrees + 22.5f) / 45.0f) % 8];
		/* Shift the angle by half a sector, divide the circle into 45-degree sectors, take the sector index, and wrap sector 8 back to sector 0.*/
}



/*
		0

	1		7
		/\
2		||		6
		||
	3		5

		4
*/
int GetDirectionDifference(Direction movementDirection, Direction viewDirection)
{
	return (static_cast<int>(movementDirection) - static_cast<int>(viewDirection) + static_cast<int>(Direction::Count)) % static_cast<int>(Direction::Count);
}