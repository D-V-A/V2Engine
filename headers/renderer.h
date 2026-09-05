#pragma once

#include <SDL3/SDL.h>
#include "types/rect.h"

class Texture;
class Window;

class Renderer
{
public:
	Renderer() = default;
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

public:
	bool Initialize(Window &window);

	void Clear();
	void FillRect(const Rect& rect);
	void Present();

	SDL_Renderer* GetNativeRenderer() const{ return m_sdlRenderer; }

	void DrawTexture(const Texture& texture, const Rect& destination);

private:
	SDL_Renderer* m_sdlRenderer = nullptr;
};