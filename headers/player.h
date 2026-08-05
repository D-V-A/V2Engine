#pragma once

#include <SDL3/SDL_rect.h>

class Renderer;
class SDL_Window;

class Player
{
public:
	void MovePlayer(float deltaTime, const bool* keyboardState);
	void Render(Renderer& renderer) const;
	void CheckScreenBorders(SDL_Window* window);

private:
	SDL_FRect rectangle{
		590.0f,
		310.0f,
		100.0f,
		100.0f
	};

	float speed = 200.0f;
};