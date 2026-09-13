#pragma once

#include <algorithm>

#include "types/rect.h"
#include "types/vector2f.h"

inline float GetRight(const Rect& rect)
{
	return rect.x() + rect.width();
}

inline float GetBottom(const Rect& rect)
{
	return rect.y() + rect.height();
}

inline bool OverlapsX(const Rect& first, const Rect& second)
{
	return GetRight(first) > second.x() && first.x() < GetRight(second);
}

inline bool OverlapsY(const Rect& first, const Rect& second)
{
	return GetBottom(first) > second.y() && first.y() < GetBottom(second);
}

inline Rect GetSweptBounds(const Rect& rect, const Vector2f& movement)
{
	Rect result;

	result.position = {
		std::min(rect.x(), rect.x() + movement.x),
		std::min(rect.y(), rect.y() + movement.y)
	};

	result.size = {
		rect.width() + std::abs(movement.x),
		rect.height() + std::abs(movement.y)
	};

	return result;
}

inline float GetDistanceSquared(const Rect& first, const Rect& second)
{
	float dx = 0.0f;
	float dy = 0.0f;

	if (GetRight(first) < second.x())
		dx = second.x() - GetRight(first);
	else if (GetRight(second) < first.x())
		dx = first.x() - GetRight(second);

	if (GetBottom(first) < second.y())
		dy = second.y() - GetBottom(first);
	else if (GetBottom(second) < first.y())
		dy = first.y() - GetBottom(second);

	return dx * dx + dy * dy;
}