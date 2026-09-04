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
	MapInitFail,
	ObjTxtFail
};

class World
{
public:
	InitializationResults Initialize(Renderer& renderer, const char* mapPath);
	void Render(Renderer& renderer, const Vector2f& origin) const;

	float GetTileWidth() const { return m_tileWidth; }
	float GetTileHeight() const { return m_tileHeight; }

	Vector2f GetSize() const { return { static_cast<float>(m_width),static_cast<float>(m_height) }; }

	const std::vector<WorldObject>& GetObjectsList() const { return m_objects; }

	float GetSpeedModifierAt(const Vector2f& position) const;

	Vector2f ResolveMovement(const Rect& collisionRect, const Vector2f& movement) const;

private:
	bool InitializeMap(Renderer& renderer, const MapData& mapInfo);

	bool InitializeObjects(Renderer& renderer, const MapData& mapInfo);
	
	const Texture& GetSurfaceTexture(SurfaceType surface) const { return m_SurfTextures.at(surface); };

	struct SweepHit
	{
		bool hit = false;
		float time = 1.0f;
		Vector2f normal{};
	};

	SweepHit SweepRect(const Rect& movingRect, const Vector2f& movement, const Rect& obstacle) const;

	SweepHit FindFirstCollision(const Rect& collisionRect, const Vector2f& movement) const;

	int m_width = 0;//map width in tiles
	int m_height = 0;//map height in tiles

	float m_tileWidth = 0.0f;//in pixels
	float m_tileHeight = 0.0f;//in pixels
	const Vector2f m_tile_pivot = { 0.5f, 0.0f };//middle, top //pivot - основание/база

	std::vector<TileData> m_tiles;
	std::map<SurfaceType, SurfaceInfo> m_surfaceTypes;
	std::map<SurfaceType, Texture> m_SurfTextures;

	std::vector<WorldObject> m_objects;
};