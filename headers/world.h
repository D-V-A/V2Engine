#pragma once

#include<vector>

#include "texture.h"
#include "world_object.h"
#include "types/vector2f.h"

class Renderer;

class World
{
public:
	bool Initialize(Renderer& renderer);
	bool InitializeObjects(Renderer& renderer, const char* texturePath);
	void Render(Renderer& renderer) const;
	void RenderObjects(Renderer& renderer) const;

	float GetTileWidth() const { return tileWidth; }
	float GetTileHeight() const { return tileHeight; }
	Vector2f GetOrigin() const { return origin; }
	Vector2f GetSize() const { return { static_cast<float>(width),static_cast<float>(height) }; }

	bool ValidatePlayerPos(Rect collisionRect) const;

private:
	bool CheckMapBorders(Rect collisionRect) const;
	void CreateObjects();

	Texture floorTexture;
	std::vector<WorldObject> objects;

	float tileWidth = 0.0f;
	float tileHeight = 0.0f;
	Vector2f origin;
	const Vector2f pivot = { 0.5f, 0.0f };//middle, top //pivot - основание/база

	int width = 10;
	int height = 10;
};