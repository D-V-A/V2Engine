#pragma once

#include "types/vector2i.h"
#include "types/rect.h"
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
	void CalculateRect();
	Rect rectangle{
		{0.0f, 0.0f},
		{0.0f, 0.0f}
	};

	float speed = 200.0f;

	Texture texture;
};