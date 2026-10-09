#include<SDL3/SDL_keyboard.h>
#include<SDL3/SDL_scancode.h>
#include<SDL3/SDL_mouse.h>

#include "core/input.h"

void Input::Update()
{
	//WASD
	m_keyboardState = SDL_GetKeyboardState(nullptr);

	m_movementDirection.x = static_cast<int>(m_keyboardState[SDL_SCANCODE_D]) - static_cast<int>(m_keyboardState[SDL_SCANCODE_A]);
	m_movementDirection.y = static_cast<int>(m_keyboardState[SDL_SCANCODE_W]) - static_cast<int>(m_keyboardState[SDL_SCANCODE_S]);
	
	//Cursor+RMB
	float mouseX = 0.0f;
	float mouseY = 0.0f;

	const SDL_MouseButtonFlags mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	m_mousePosition = { mouseX, mouseY };
	m_aimState = static_cast<bool>(mouseState & SDL_BUTTON_RMASK);

	//Shift
	m_shiftState = static_cast<bool>(m_keyboardState[SDL_SCANCODE_LSHIFT]) || static_cast<bool>(m_keyboardState[SDL_SCANCODE_RSHIFT]);

	//E
	const bool interactionState = static_cast<bool>(m_keyboardState[SDL_SCANCODE_E]);

	m_interactionPressed = interactionState && !m_previousInteractionState;
	m_previousInteractionState = interactionState;

}
