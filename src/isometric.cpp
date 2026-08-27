#include "isometric.h"

Vector2f WorldToScreen(const Vector2f& worldPosition, const Vector2f& size /*tile size*/, const Vector2f& origin)
{
	return{	origin.x + (worldPosition.x - worldPosition.y) * size.x / 2.0f,
			origin.y + (worldPosition.x + worldPosition.y) * size.y / 2.0f };
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