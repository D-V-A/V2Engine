#pragma once

#include <SDL3/SDL.h>
#include "types/rect.h"

class Texture;

class Renderer
{
public:
	Renderer() = default;
	~Renderer();

public:
	bool Initialize(SDL_Window* window);

	void Clear();
	void FillRect(const Rect& rect);
	void Present();

	SDL_Renderer* GetNativeRenderer() const{ return sdlRenderer; }

	void DrawTexture(const Texture& texture, const Rect& destination);

private:
	SDL_Renderer* m_sdlRenderer = nullptr;
};