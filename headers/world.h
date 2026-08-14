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

	float GetTileWidth() const { return m_tileWidth; }
	float GetTileHeight() const { return m_tileHeight; }
	Vector2f GetOrigin() const { return m_origin; }
	Vector2f GetSize() const { return { static_cast<float>(m_width),static_cast<float>(m_height) }; }

	Vector2f ResolveMovement(const Rect& collisionRect, const Vector2f& movement) const;

private:
	float ResolveMovementX(const Rect& collisionRect, float movement) const;
	float ResolveMovementY(const Rect& collisionRect, float movement) const;

	void CreateObjects();

	Texture m_floorTexture;
	std::vector<WorldObject> m_objects;

	float m_tileWidth = 0.0f;
	float m_tileHeight = 0.0f;
	Vector2f m_origin;
	const Vector2f m_pivot = { 0.5f, 0.0f };//middle, top //pivot - основание/база

	int m_width = 10;
	int m_height = 10;
};