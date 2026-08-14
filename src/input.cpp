#include<SDL3/SDL_keyboard.h>

#include "input.h"

void Input::Update()
{
	m_keyboardState = SDL_GetKeyboardState(nullptr);

	m_movementDirection.x = (int)m_keyboardState[SDL_SCANCODE_D] - (int)m_keyboardState[SDL_SCANCODE_A];
	m_movementDirection.y = (int)m_keyboardState[SDL_SCANCODE_W] - (int)m_keyboardState[SDL_SCANCODE_S];
	
}

Vector2i Input::GetDirection() const
{ 
	return m_movementDirection;
};