#include <SDL3/SDL.h>

#include "core/sdl_context.h"

bool SDLContext::Initialize()
{
	return SDL_Init(SDL_INIT_VIDEO);
}

SDLContext::~SDLContext()
{
	SDL_Quit();
}