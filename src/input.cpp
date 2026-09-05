#include<SDL3/SDL_keyboard.h>
#include<SDL3/SDL_mouse.h>

#include "input.h"

void Input::Update()
{
	m_keyboardState = SDL_GetKeyboardState(nullptr);

	m_movementDirection.x = (int)m_keyboardState[SDL_SCANCODE_D] - (int)m_keyboardState[SDL_SCANCODE_A];
	m_movementDirection.y = (int)m_keyboardState[SDL_SCANCODE_W] - (int)m_keyboardState[SDL_SCANCODE_S];
	
	float mouseX = 0.0f;
	float mouseY = 0.0f;

	SDL_GetMouseState(&mouseX, &mouseY);

	m_mousePosition = { mouseX, mouseY };
}