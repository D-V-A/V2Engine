#include <SDL3/SDL.h>
#include "rect.h"

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

private:
	SDL_Renderer* sdlRenderer = nullptr;
};