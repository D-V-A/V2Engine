#pragma once

#include "types/vector2i.h"
#include "types/rect.h"
#include "texture.h"
class Renderer;

class Player
{
public:
	bool Initialize(Renderer& renderer);

	void MovePlayer(float deltaTime, Vector2i direction);
	void CheckMapBorders(const Vector2f& worldSize);

	void Render(Renderer& renderer, const Vector2f& screenPosition) const;

	Vector2f GetPosition() const { return position;	}

private:
	void CalculateRectSize();
	Rect textureRectangle{
		{0.0f, 0.0f},
		{0.0f, 0.0f}
	};

	Vector2f position{ 0.5f, 0.5f };
	Vector2f pivot = { 0.5f, 1.0f };//middle, bottom

	Texture texture;
};