#include <algorithm>
#include <stdlib.h>

#include "SDL3/SDL.h"
#include "player.h"
#include "renderer.h"

void Player::MovePlayer(float deltaTime, const Vector2i direction)
{
	const float speed = (direction.x != 0 && direction.y != 0) ? 141.42136f : 200.0f; // пикселей в секунду
	
	rectangle.x += speed * direction.x * deltaTime;;
	rectangle.y -= speed * direction.y * deltaTime;;
}

void Player::Render(Renderer& renderer) const
{
	renderer.FillRect(rectangle);
}

void Player::CheckScreenBorders(SDL_Window* window)
{
	int windowWidth;
	int windowHeight;

	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	rectangle.x = std::clamp(
		rectangle.x,
		0.0f,
		windowWidth - rectangle.w);
	rectangle.y = std::clamp(
		rectangle.y,
		0.0f,
		windowHeight - rectangle.h);
}