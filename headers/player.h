#pragma once

#include "types/vector2i.h"
#include "types/rect.h"
#include "texture.h"
class Renderer;

class Player
{
public:
	bool Initialize(Renderer& renderer);

	Vector2f CalculatePlayerMovement(float deltaTime, Vector2i direction) const;
	void Render(Renderer& renderer, const Vector2f& screenPosition) const;

	void SetPosition(const Vector2f& newPosition) { position = newPosition; };
	Vector2f GetPosition() const { return position;	}
	Rect GetCollisionRectAt(const Vector2f& position) const;

private:
	Texture texture;

	void CalculateRectSize();
	Rect textureRectangle{
		{0.0f, 0.0f},
		{0.0f, 0.0f} };

	Vector2f position{ 3.5f, 3.5f };//in world coordinates
	Vector2f pivot = { 0.5f, 1.0f };//middle, bottom

	Rect collisionRect{
		{ -0.25f, -0.25f },//relative to map position
		{0.5f, 0.5f} };//collision size
};