#include "renderer.h"

bool Renderer::Initialize(SDL_Window* window)
{
	sdlRenderer = SDL_CreateRenderer(window, nullptr);
	if (sdlRenderer == nullptr)
		return false;
	return true;
}

Renderer::~Renderer()
{
	if (sdlRenderer != nullptr)
	{
		SDL_DestroyRenderer(sdlRenderer);
		sdlRenderer = nullptr;
	}
}

void Renderer::Clear()
{
	SDL_SetRenderDrawColor(sdlRenderer, 30, 30, 35, 255);
	SDL_RenderClear(sdlRenderer);
}

void Renderer::FillRect(const SDL_FRect& rect)
{

	// Цвет прямоугольника: оранжевый.
	SDL_SetRenderDrawColor(sdlRenderer, 220, 120, 40, 255);
	SDL_RenderFillRect(sdlRenderer, &rect);
}

void Renderer::Present()
{
	// Показываем готовый кадр на экране.
	SDL_RenderPresent(sdlRenderer);
}
