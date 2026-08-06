#pragma once

#include <SDL3/SDL_rect.h>

#include "vector2i.h"

class Renderer;
class SDL_Window;

class Player
{
public:
	void MovePlayer(float deltaTime, const Vector2i direction);
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