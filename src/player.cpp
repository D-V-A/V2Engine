#include <algorithm>

#include "SDL3/SDL.h"
#include "player.h"
#include "renderer.h"

void Player::MovePlayer(float deltaTime, const bool* keyboardState)
{
	constexpr float speed = 200.0f; // пикселей в секунду

	if (keyboardState[SDL_SCANCODE_W])
	{
		rectangle.y -= speed * deltaTime;
	}

	if (keyboardState[SDL_SCANCODE_S])
	{
		rectangle.y += speed * deltaTime;
	}

	if (keyboardState[SDL_SCANCODE_A])
	{
		rectangle.x -= speed * deltaTime;
	}

	if (keyboardState[SDL_SCANCODE_D])
	{
		rectangle.x += speed * deltaTime;
	}
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