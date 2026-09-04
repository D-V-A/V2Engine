#include <algorithm>
#include <cassert>
#include <limits>
#include <cmath>

#include "world.h"
#include "renderer.h"
#include "isometric.h"
#include "map_loader.h"
#include "assets.h"

#include "types/rect.h"
#include "types/map_data.h"

InitializationResults World::Initialize(Renderer& renderer, const char* mapPath)
{
	MapData mapLoadData;

	if (!MapLoader::Load(mapPath, mapLoadData))
		return InitializationResults::InfoLoadFail;

	if (!InitializeMap(renderer, mapLoadData))
		return InitializationResults::MapInitFail;

	if (!InitializeObjects(renderer, mapLoadData))
		return InitializationResults::ObjTxtFail;

	return InitializationResults::Success;
}

bool World::InitializeMap(Renderer& renderer, const MapData& mapInfo)
{
	//TO DO: Do somexting with textures initialization
	for (auto it = mapInfo.surfaceTypes.cbegin(); it != mapInfo.surfaceTypes.cend(); it++)
	{
		Texture txt;

		auto assetPath = GetAssetPath(it->second.texture);
		if (!txt.Load(renderer, assetPath.string().c_str()))
			return false;
		
		const auto [iter, inserted] = m_SurfTextures.emplace(it->first, std::move(txt));

		if (!inserted)
			return false;
	}

	m_tileWidth = static_cast<float>(m_SurfTextures.begin()->second.GetWidth());
	m_tileHeight = static_cast<float>(m_SurfTextures.begin()->second.GetHeight());

	m_width = mapInfo.width;
	m_height = mapInfo.height;

	m_tiles = std::move(mapInfo.tiles);
	m_surfaceTypes = std::move(mapInfo.surfaceTypes);

	constexpr float windowWidth = 1280.0f;//todo: delete const
	constexpr float windowHeight = 720.0f;

	return true;
}

bool World::InitializeObjects(Renderer& renderer, const MapData& mapInfo)
{
	m_objects.clear();
	m_objects.reserve(mapInfo.objects.size());

	for (const ObjectInstanceData& objectInstance : mapInfo.objects)
	{
		const ObjectTypeData& typeData = mapInfo.objectTypes.at(objectInstance.type);

		WorldObject object(
			objectInstance.position,
			typeData.renderFootprintSize,
			typeData.collision);

		std::filesystem::path texturePath;

		switch (objectInstance.type)
		{
		case ObjectType::Crate:
			texturePath = GetAssetPath("txt/crate.png");
			break;

		case ObjectType::Tree:
			texturePath = GetAssetPath("txt/tree.png");
			break;

		case ObjectType::Rock:
			texturePath = GetAssetPath("txt/rock.png");
			break;

		case ObjectType::Bush:
			texturePath = GetAssetPath("txt/bush.png");
			break;
		}
		
		if (!object.Initialize(renderer, texturePath.string().c_str()))
			return false;

		m_objects.push_back(std::move(object));
	}

	return true;
}

void World::Render(Renderer& renderer, const Vector2f& origin) const
{
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			const size_t index = static_cast<size_t>(y * m_width + x);
			const TileData& tile = m_tiles[index];

			const Texture& texture = GetSurfaceTexture(tile.surface);
			
			Rect tileRect;

			tileRect.size = { m_tileWidth, m_tileHeight };

			const Vector2f tileCenter =	WorldToScreen( { static_cast<float>(x), static_cast<float>(y) }, tileRect.size, origin);

			tileRect.position = GetTopLeft(tileCenter, tileRect.size, m_tile_pivot);

			renderer.DrawTexture(
				texture,
				tileRect);
		}
	}
}

float World::GetSpeedModifierAt(const Vector2f& position) const
{
	const int x = static_cast<int>(std::floor(position.x));
	const int y = static_cast<int>(std::floor(position.y));

	if (x < 0 || x >= m_width ||
		y < 0 || y >= m_height)
	{
		return 1.0f;
	}

	const size_t index =
		static_cast<size_t>(y * m_width + x);

	const SurfaceType surface = m_tiles[index].surface;
	const SurfaceInfo& info = m_surfaceTypes.at(surface);

	return info.speedModificator.value_or(1.0f);
}

Vector2f World::ResolveMovement(const Rect& collisionRect, const Vector2f& movement) const
{
	Vector2f result{};
	Vector2f remainingMovement = movement;

	Rect currentRect = collisionRect;

	constexpr int maxCollisions = 2;

	for (int i = 0; i < maxCollisions; ++i)
	{
		if (remainingMovement.x == 0.0f
			&& remainingMovement.y == 0.0f)
		{
			break;
		}

		const SweepHit hit =
			FindFirstCollision(currentRect, remainingMovement);

		if (!hit.hit)
		{
			result.x += remainingMovement.x;
			result.y += remainingMovement.y;

			break;
		}

		const Vector2f allowedMovement{
			remainingMovement.x * hit.time,
			remainingMovement.y * hit.time
		};

		result.x += allowedMovement.x;
		result.y += allowedMovement.y;

		currentRect.x() += allowedMovement.x;
		currentRect.y() += allowedMovement.y;

		remainingMovement.x *= 1.0f - hit.time;
		remainingMovement.y *= 1.0f - hit.time;

		// Убираем компонент движения,
		// направленный в сторону столкновения.

		if (hit.normal.x != 0.0f)
			remainingMovement.x = 0.0f;

		if (hit.normal.y != 0.0f)
			remainingMovement.y = 0.0f;
	}

	return result;
}

World::SweepHit World::FindFirstCollision(const Rect& collisionRect, const Vector2f& movement) const
{
	SweepHit nearestHit;

	const Rect worldBounds[] = {
		Rect //leftBound
		{{ -1.0f, -1.0f },
		{ 1.0f, static_cast<float>(m_height) + 2.0f }},
		Rect //rightBound
		{{ static_cast<float>(m_width), -1.0f },
		{ 1.0f, static_cast<float>(m_height) + 2.0f }},
		Rect //topBound
		{{ -1.0f, -1.0f },
		{ static_cast<float>(m_width) + 2.0f, 1.0f }},
		Rect //bottomBound
		{{ -1.0f, static_cast<float>(m_height) },
		{ static_cast<float>(m_width) + 2.0f, 1.0f }}
	};

	for (const Rect& bound : worldBounds)
	{
		const SweepHit hit = SweepRect(collisionRect, movement, bound);

		if (hit.hit && hit.time < nearestHit.time)
			nearestHit = hit;
	}

	constexpr float epsilon = 0.0001f;

	// --------------------------------
	// Terrain
	// --------------------------------

	const float endX = collisionRect.x() + movement.x;
	const float endY = collisionRect.y() + movement.y;

	const float minX = std::min(collisionRect.x(), endX);
	const float minY = std::min(collisionRect.y(), endY);

	const float maxX = std::max(collisionRect.x() + collisionRect.width(), endX + collisionRect.width());

	const float maxY = std::max(collisionRect.y() + collisionRect.height(), endY + collisionRect.height());

	const int minTileX = std::max(0, static_cast<int>(std::floor(minX)));

	const int minTileY = std::max(0, static_cast<int>(std::floor(minY)));

	const int maxTileX = std::min(m_width - 1, static_cast<int>(std::floor(maxX - epsilon)));

	const int maxTileY = std::min(m_height - 1, static_cast<int>(std::floor(maxY - epsilon)));

	for (int y = minTileY; y <= maxTileY; ++y)
	{
		for (int x = minTileX; x <= maxTileX; ++x)
		{
			const size_t index =
				static_cast<size_t>(y * m_width + x);

			const TileData& tile = m_tiles[index];

			const SurfaceInfo& surface =
				m_surfaceTypes.at(tile.surface);

			if (surface.walkable)
				continue;

			Rect tileRect{
				{static_cast<float>(x),	static_cast<float>(y)},
				{ 1.0f, 1.0f }
			};

			const SweepHit hit =
				SweepRect(collisionRect, movement, tileRect);

			if (hit.hit && hit.time < nearestHit.time)
				nearestHit = hit;
		}
	}

	// --------------------------------
	// Objects
	// --------------------------------

	for (const WorldObject& object : m_objects)
	{
		if (!object.HasCollision())
			continue;

		const SweepHit hit = SweepRect(collisionRect, movement, object.GetCollisionRect());

		if (hit.hit && hit.time < nearestHit.time)
			nearestHit = hit;
	}

	return nearestHit;
}

World::SweepHit World::SweepRect(const Rect& movingRect, const Vector2f& movement, const Rect& obstacle) const
{
	SweepHit result;

	float xEntry;
	float xExit;
	float yEntry;
	float yExit;

	if (movement.x > 0.0f)
	{
		xEntry = (obstacle.x() - (movingRect.x() + movingRect.width())) / movement.x;

		xExit = ((obstacle.x() + obstacle.width()) - movingRect.x()) / movement.x;
	}
	else if (movement.x < 0.0f)
	{
		xEntry = ((obstacle.x() + obstacle.width()) - movingRect.x()) / movement.x;

		xExit = (obstacle.x() - (movingRect.x() + movingRect.width())) / movement.x;
	}
	else
	{
		if (movingRect.x() + movingRect.width() <= obstacle.x()	
			|| movingRect.x() >= obstacle.x() + obstacle.width())
		{
			return result;
		}

		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}

	if (movement.y > 0.0f)
	{
		yEntry = (obstacle.y() - (movingRect.y() + movingRect.height())) / movement.y;

		yExit = ((obstacle.y() + obstacle.height()) - movingRect.y()) / movement.y;
	}
	else if (movement.y < 0.0f)
	{
		yEntry = ((obstacle.y() + obstacle.height()) - movingRect.y()) / movement.y;

		yExit = (obstacle.y() - (movingRect.y() + movingRect.height())) / movement.y;
	}
	else
	{
		if (movingRect.y() + movingRect.height() <= obstacle.y() 
			|| movingRect.y() >= obstacle.y() + obstacle.height())
		{
			return result;
		}

		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	}

	const float entryTime = std::max(xEntry, yEntry);
	const float exitTime = std::min(xExit, yExit);

	if (entryTime > exitTime
		|| entryTime < 0.0f
		|| entryTime > 1.0f)
	{
		return result;
	}

	result.hit = true;
	result.time = entryTime;

	if (xEntry > yEntry)
	{
		result.normal.x = movement.x > 0.0f ? -1.0f : 1.0f;
	}
	else
	{
		result.normal.y = movement.y > 0.0f ? -1.0f : 1.0f;
	}

	return result;
}