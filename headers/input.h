#pragma once

#include "types/vector2i.h"

class Input
{
public:
	void Update();

	Vector2i GetDirection() const;

private:
	Vector2i m_movementDirection{ 0,0 };
	const bool* m_keyboardState = nullptr;
};