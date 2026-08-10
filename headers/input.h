#pragma once

#include "types/vector2i.h"

class Input
{
public:
	void Update();

	Vector2i GetDirection() const;

private:
	Vector2i movementDirection{ 0,0 };
	const bool* keyboardState = nullptr;
};