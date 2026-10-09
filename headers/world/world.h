#pragma once

#include<vector>
#include <memory>

#include "graphics/texture.h"

#include "entities/world_object.h"

#include "types/vector2f.h"
#include "types/map_data.h"

class ResourceManager;
class Renderer;

enum InitializationResult
{
	Success,
	InfoLoadFail,
	MapInitFail,
	ObjectInitFail
};

class World
{
public:
	InitializationResult Initialize(ResourceManager& resManager, const char* mapPath);
	void Render(Renderer& renderer, const Vector2f& origin) const;

public:
	float GetTileWidth() const { return m_tileWidth; }
	float GetTileHeight() const { return m_tileHeight; }

	Vector2f GetSize() const { return { static_cast<float>(m_width),static_cast<float>(m_height) }; }

	const std::vector<WorldObject>& GetObjectsList() const { return m_objects; }

	float GetSpeedModifierAt(const Vector2f& position) const;

	Vector2f ResolveMovement(const Rect& collisionRect, const Vector2f& movement) const;
	const WorldObject* FindInteractionTarget(const Rect& interactionSource) const;

	bool Interact(const Rect& interactionSource);
private:
	bool InitializeMap(ResourceManager& resManager, MapData& mapInfo);

	bool InitializeObjects(ResourceManager& resManager, const MapData& mapInfo);

	const Texture& GetSurfaceTexture(SurfaceType surface) const { return *m_surfaceTextures.at(surface); }

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
	const Vector2f m_tilePivot = { 0.5f, 0.0f };//middle, top //pivot - основание/база

	std::vector<TileData> m_tiles;
	std::map<SurfaceType, SurfaceInfo> m_surfaceTypes;
	std::map<SurfaceType, std::shared_ptr<Texture>> m_surfaceTextures;

	std::vector<WorldObject> m_objects;
	std::map<std::string, std::shared_ptr<Texture>> m_objectTextures;
};