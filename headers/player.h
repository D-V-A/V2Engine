#pragma once

#include "vector2i.h"
#include "rect.h"
#include "texture.h"
class Renderer;
class SDL_Window;

class Player
{
public:
	bool Initialize(Renderer& renderer);

	void MovePlayer(float deltaTime, Vector2i direction);
	void Render(Renderer& renderer) const;
	void CheckScreenBorders(int width, int height);

private:
	Rect rectangle{
		{440.0f, 260.0f},
		{100.0f, 100.0f}
	};

	float speed = 200.0f;

	Texture texture;
};