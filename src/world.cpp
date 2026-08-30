#include <algorithm>
#include <cmath>
#include <cassert>

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

	m_origin.x = windowWidth / 2;
	m_origin.y = (windowHeight - mapInfo.height * m_tileHeight) / 2;

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

void World::Render(Renderer& renderer) const
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

			const Vector2f tileCenter =	WorldToScreen( { static_cast<float>(x), static_cast<float>(y) }, tileRect.size, m_origin);

			tileRect.position = GetTopLeft(tileCenter, tileRect.size, m_tile_pivot);

			renderer.DrawTexture(
				texture,
				tileRect);
		}
	}
}

Vector2f World::ResolveMovement(const Rect& collisionRect, const Vector2f& movement) const
{
	Vector2f result = movement;
	Rect movedRect = collisionRect;

	result.x = ResolveMovementX(collisionRect, movement.x);
	movedRect.x() += result.x;

	result.y = ResolveMovementY(movedRect, movement.y);

	return result;
}

float World::ResolveMovementX(const Rect& collisionRect, float movement) const
{
	auto OverlapsY = [](const Rect& a, const Rect& b)
		{
			return a.y() < b.y() + b.height() && a.y() + a.height() > b.y();
		};

	float allowedMovement = movement;

	if (movement > 0.0f)
	{
		const float worldRight = static_cast<float>(m_width);
		const float playerRight = collisionRect.x() + collisionRect.width();
		allowedMovement = std::min(allowedMovement, worldRight - playerRight);

		for (const WorldObject& object : m_objects)
		{
			if (!object.HasCollision())
				continue;

			const Rect objectRect = object.GetCollisionRect();

			if (!OverlapsY(collisionRect, objectRect))
				continue;

			if (playerRight <= objectRect.x())
			{
				const float distance = objectRect.x() - playerRight;

				if (distance < allowedMovement)
					allowedMovement = distance;
			}
		}
	}
	else if (movement < 0.0f)
	{
		const float playerLeft = collisionRect.x();
		allowedMovement = -std::min(std::abs(allowedMovement), playerLeft);//worldLeft == 0

		for (const WorldObject& object : m_objects)
		{
			if (!object.HasCollision())
				continue;

			const Rect objectRect = object.GetCollisionRect();

			if (!OverlapsY(collisionRect, objectRect))
				continue;

			if (playerLeft >= objectRect.x() + objectRect.width())
			{
				const float distance = playerLeft - (objectRect.x() + objectRect.width()) ;

				if (distance < std::abs(allowedMovement))
					allowedMovement = -distance;
			}
		}
	}

	return allowedMovement;
}

float World::ResolveMovementY(const Rect& collisionRect, float movement) const
{
	auto OverlapsX = [](const Rect& a, const Rect& b)
		{
			return a.x() < b.x() + b.width() && a.x() + a.width() > b.x();
		};

	float allowedMovement = movement;

	if (movement > 0.0f)
	{
		const float worldBottom = static_cast<float>(m_height);
		const float playerBottom = collisionRect.y() + collisionRect.height();
		allowedMovement = std::min(allowedMovement, worldBottom - playerBottom);

		for (const WorldObject& object : m_objects)
		{
			if (!object.HasCollision())
				continue;

			const Rect objectRect = object.GetCollisionRect();

			if (!OverlapsX(collisionRect, objectRect))
				continue;

			if (playerBottom <= objectRect.y())
			{
				const float distance = objectRect.y() - playerBottom;

				if (distance < allowedMovement)
					allowedMovement = distance;
			}
		}
	}
	else if (movement < 0.0f)
	{
		const float playerTop = collisionRect.y();
		allowedMovement = - std::min(std::abs(allowedMovement), playerTop);//worldTop == 0

		for (const WorldObject& object : m_objects)
		{
			if (!object.HasCollision())
				continue;

			const Rect objectRect = object.GetCollisionRect();

			if (!OverlapsX(collisionRect, objectRect))
				continue;

			if (playerTop >= objectRect.y() + objectRect.height())
			{
				const float distance = playerTop - (objectRect.y() + objectRect.height());

				if (distance < std::abs(allowedMovement))
					allowedMovement = -distance;
			}
		}
	}

	return allowedMovement;
}

const Texture& World::GetSurfaceTexture(SurfaceType surface) const
{
	return m_SurfTextures.at(surface);
}