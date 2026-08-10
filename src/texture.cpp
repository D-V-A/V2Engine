#include <iostream> 

#include <SDL3/SDL_render.h>

#include "texture.h"
#include "renderer.h"

bool Texture::Load(Renderer& renderer, const char* path)
{
	SDL_Surface* surface = SDL_LoadBMP("assets/txt/player.bmp");//path);

	if (surface == nullptr)
	{
		return false;
	}

	sdlTexture = SDL_CreateTextureFromSurface(
		renderer.GetNativeRenderer(),
		surface);

	SDL_DestroySurface(surface);

	return sdlTexture != nullptr;
}

Texture::~Texture()
{
	if (sdlTexture != nullptr)
	{
		SDL_DestroyTexture(sdlTexture);
	}
}

SDL_Texture* Texture::GetNativeTexture() const
{
	return sdlTexture;
}