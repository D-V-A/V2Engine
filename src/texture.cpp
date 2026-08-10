#include <iostream> 

#include <SDL3/SDL_render.h>

#include "texture.h"
#include "renderer.h"

bool Texture::Load(Renderer& renderer, const char* path)
{
	SDL_Surface* surface = SDL_LoadBMP("assets/txt/player.bmp");//path);

	if (surface == nullptr)
		return false;

	sdlTexture = SDL_CreateTextureFromSurface(
		renderer.GetNativeRenderer(),
		surface);

	if (!sdlTexture)
		return false;

	float textureWidth = 0.0f;
	float textureHeight = 0.0f;
	if (!SDL_GetTextureSize(sdlTexture, &textureWidth, &textureHeight))
		return false;
	width = static_cast<int>(textureWidth);
	height = static_cast<int>(textureHeight);

	SDL_DestroySurface(surface);

	return true;
}

int Texture::GetWidth() const { return width; }
int Texture::GetHeight() const { return height; }

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