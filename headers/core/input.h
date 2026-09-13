#pragma once

#include "types/vector2i.h"
#include "types/vector2f.h"

class Input
{
public:
	void Update();
	
	Vector2i GetDirection() const { return m_movementDirection; }
	Vector2f GetMousePosition() const { return m_mousePosition; }
	bool GetShiftState() const { return m_shiftState; }

	bool GetInteractionPressed() const { return m_interactionPressed; }

private:
	Vector2i m_movementDirection{ 0,0 };
	const bool* m_keyboardState = nullptr;

	Vector2f m_mousePosition;

	bool m_shiftState = false;

	bool m_interactionPressed = false;
	bool m_previousInteractionState = false;
};