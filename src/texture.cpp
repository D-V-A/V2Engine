#include <iostream> 

#include <SDL3/SDL_render.h>

#include "texture.h"
#include "renderer.h"

bool Texture::Load(Renderer& renderer, const char* path)
{
	SDL_Surface* surface = SDL_LoadBMP(path);

	if (surface == nullptr)
		return false;

	m_sdlTexture = SDL_CreateTextureFromSurface(
		renderer.GetNativeRenderer(),
		surface);

	SDL_DestroySurface(surface);

	if (!m_sdlTexture)
		return false;

	float textureWidth = 0.0f;
	float textureHeight = 0.0f;
	if (!SDL_GetTextureSize(m_sdlTexture, &textureWidth, &textureHeight))
	{
		SDL_DestroyTexture(m_sdlTexture);
		m_sdlTexture = nullptr;
		return false;
	}
	m_width = static_cast<int>(textureWidth);
	m_height = static_cast<int>(textureHeight);

	return true;
}

Texture::~Texture()
{
	if (m_sdlTexture != nullptr)
	{
		SDL_DestroyTexture(m_sdlTexture);
	}
}

SDL_Texture* Texture::GetNativeTexture() const
{
	return m_sdlTexture;
}