#pragma once

#include "types/vector2i.h"
#include "types/vector2f.h"

class Input
{
public:
	void Update();
	
	Vector2i GetDirection() const { return m_movementDirection; };
	Vector2f GetMousePosition() const { return m_mousePosition; }

private:
	Vector2i m_movementDirection{ 0,0 };
	const bool* m_keyboardState = nullptr;

	Vector2f m_mousePosition;
};