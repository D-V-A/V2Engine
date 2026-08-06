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

void Renderer::FillRect(const Rect& rect)
{
	const SDL_FRect sdlRectangle{
		rect.position.x,
		rect.position.y,
		rect.size.x,
		rect.size.y
	};
	// Цвет прямоугольника: оранжевый.
	SDL_SetRenderDrawColor(sdlRenderer, 220, 120, 40, 255);
	SDL_RenderFillRect(sdlRenderer, &sdlRectangle);
}

void Renderer::Present()
{
	// Показываем готовый кадр на экране.
	SDL_RenderPresent(sdlRenderer);
}
