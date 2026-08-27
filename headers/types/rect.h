#pragma once

#include "types/vector2f.h"

struct Rect
{
	Vector2f position;
	Vector2f size;

	float& x() { return position.x; }
	float& y() { return position.y; }
	float& width() { return size.x; }
	float& height() { return size.y; }

	const float& x() const { return position.x; }
	const float& y() const { return position.y; }
	const float& width() const { return size.x; }
	const float& height() const { return size.y; }
};