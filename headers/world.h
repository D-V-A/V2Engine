#pragma once

#include "texture.h"
#include "types/vector2f.h"

class Renderer;

class World
{
public:
	bool Initialize(Renderer& renderer);
	void Render(Renderer& renderer) const;

	float GetTileWidth() const { return tileWidth; }
	float GetTileHeight() const { return tileHeight; }
	Vector2f GetOrigin() const { return origin; }
	Vector2f GetSize() const { return { static_cast<float>(width),static_cast<float>(height) }; }

private:
	Texture floorTexture;

	float tileWidth = 0.0f;
	float tileHeight = 0.0f;
	Vector2f origin;
	const Vector2f pivot = { 0.5f, 0.5f };//middle, middle

	int width = 10;
	int height = 10;
};