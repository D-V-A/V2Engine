#include "renderer.h"
#include "texture.h"

bool Renderer::Initialize(SDL_Window* window)
{
	m_sdlRenderer = SDL_CreateRenderer(window, nullptr);
	if (m_sdlRenderer == nullptr)
		return false;
	return true;
}

Renderer::~Renderer()
{
	if (m_sdlRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_sdlRenderer);
		m_sdlRenderer = nullptr;
	}
}

void Renderer::Clear()
{
	SDL_SetRenderDrawColor(m_sdlRenderer, 30, 30, 35, 255);
	SDL_RenderClear(m_sdlRenderer);
}

void Renderer::FillRect(const Rect& rect)
{
	const SDL_FRect sdlRectangle{
		rect.x(),
		rect.y(),
		rect.width(),
		rect.height()
	};
	// Цвет прямоугольника: оранжевый.
	SDL_SetRenderDrawColor(m_sdlRenderer, 220, 120, 40, 255);
	SDL_RenderFillRect(m_sdlRenderer, &sdlRectangle);
}

void Renderer::DrawTexture(const Texture& texture, const Rect& destination)
{
	const SDL_FRect dst{
		destination.x(),
		destination.y(),
		destination.width(),
		destination.height()
	};

	SDL_RenderTexture(
		m_sdlRenderer,
		texture.GetNativeTexture(),
		nullptr,
		&dst);
}

void Renderer::Present()
{
	// Показываем готовый кадр на экране.
	SDL_RenderPresent(m_sdlRenderer);
}