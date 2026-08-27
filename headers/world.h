#pragma once

#include<vector>

#include "texture.h"
#include "world_object.h"

#include "types/vector2f.h"
#include "types/map_data.h"

class Renderer;

enum InitializationResults
{
	Success,
	InfoLoadFail,
	MapTxtFail,
	ObjTxtFail
};

class World
{
public:
	InitializationResults Initialize(Renderer& renderer, const char* mapPath);
	bool InitializeObjects(Renderer& renderer);
	void Render(Renderer& renderer) const;

	float GetTileWidth() const { return m_tileWidth; }
	float GetTileHeight() const { return m_tileHeight; }
	const Texture& GetSurfaceTexture(SurfaceType surface) const;

	Vector2f GetOrigin() const { return m_origin; }
	Vector2f GetSize() const { return { static_cast<float>(m_mapData.width),static_cast<float>(m_mapData.height) }; }

	const std::vector<WorldObject>& GetObjectsList() const { return m_objects; }

	Vector2f ResolveMovement(const Rect& collisionRect, const Vector2f& movement) const;

private:
	float ResolveMovementX(const Rect& collisionRect, float movement) const;
	float ResolveMovementY(const Rect& collisionRect, float movement) const;

	MapData m_mapData;

	Texture m_grassTexture;
	Texture m_dirtTexture;
	Texture m_waterTexture;

	std::vector<WorldObject> m_objects;

	float m_tileWidth = 0.0f;
	float m_tileHeight = 0.0f;
	Vector2f m_origin;
	const Vector2f m_pivot = { 0.5f, 0.0f };//middle, top //pivot - основание/база
};