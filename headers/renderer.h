#include <SDL3/SDL.h>

struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a = 255;
};

class Renderer
{
public:
	Renderer() = default;
	~Renderer();

public:
	bool Initialize(SDL_Window* window);

	void Clear();
	void FillRect(const SDL_FRect& rect);
	void Present();

private:
	SDL_Renderer* sdlRenderer = nullptr;
};