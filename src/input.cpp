#include<SDL3/SDL_keyboard.h>

#include "input.h"

void Input::Update()
{
	keyboardState = SDL_GetKeyboardState(nullptr);

	movementDirection.x = (int)keyboardState[SDL_SCANCODE_D] - (int)keyboardState[SDL_SCANCODE_A];
	movementDirection.y = (int)keyboardState[SDL_SCANCODE_W] - (int)keyboardState[SDL_SCANCODE_S];
	
}

Vector2i Input::GetDirection() const
{ 
	return movementDirection; 
};