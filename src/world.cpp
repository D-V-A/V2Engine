#include <algorithm>
#include <cmath>
#include <cassert>

#include "world.h"
#include "renderer.h"
#include "isometric.h"
#include "map_loader.h"
#include "assets.h"

#include "types/rect.h"

InitializationResults World::Initialize(Renderer& renderer, const char* mapPath)
{
	if (!MapLoader::Load(mapPath, m_mapData))
		return InitializationResults::InfoLoadFail;

	auto assetPath = GetAssetPath("txt/grass_tile.png");
	if (!m_grassTexture.Load(renderer, assetPath.string().c_str()))
		return InitializationResults::MapTxtFail;

	assetPath = GetAssetPath("txt/dirt_tile.png");
	if (!m_dirtTexture.Load(renderer, assetPath.string().c_str()))
		return InitializationResults::MapTxtFail;

	assetPath = GetAssetPath("txt/water_tile.png");
	if (!m_waterTexture.Load(renderer, assetPath.string().c_str()))
		return InitializationResults::MapTxtFail;

	m_tileWidth = static_cast<float>(m_grassTexture.GetWidth());

	m_tileHeight = static_cast<float>(m_grassTexture.GetHeight());

	constexpr float windowWidth = 1280.0f;//todo: delete const
	constexpr float windowHeight = 720.0f;

	m_origin.x = windowWidth / 2;
	m_origin.y = (windowHeight - m_mapData.height * m_tileHeight) / 2;

	if (!InitializeObjects(renderer))
		return InitializationResults::ObjTxtFail;

	return InitializationResults::Success;
}

bool World::InitializeObjects(Renderer& renderer)
{
	m_objects.clear();
	m_objects.reserve(m_mapData.objects.size());

	for (const ObjectInstanceData& objectInstance : m_mapData.objects)
	{
		const ObjectTypeData& typeData = m_mapData.objectTypes.at(objectInstance.type);

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
	for (int y = 0; y < m_mapData.height; ++y)
	{
		for (int x = 0; x < m_mapData.width; ++x)
		{
			const size_t index = static_cast<size_t>(y * m_mapData.width + x);
			const TileData& tile = m_mapData.tiles[index];

			const Texture& texture = GetSurfaceTexture(tile.surface);
			
			Rect tileRect;

			tileRect.size = { m_tileWidth, m_tileHeight };

			const Vector2f tileCenter =	WorldToScreen( { static_cast<float>(x), static_cast<float>(y) }, tileRect.size, m_origin);

			tileRect.position = GetTopLeft(tileCenter, tileRect.size, m_pivot);

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
		const float worldRight = static_cast<float>(m_mapData.width);
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
		const float worldBottom = static_cast<float>(m_mapData.height);
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
	switch (surface)
	{
	case SurfaceType::Grass:
		return m_grassTexture;

	case SurfaceType::Dirt:
		return m_dirtTexture;

	case SurfaceType::Water:
		return m_waterTexture;
	}

	assert(false);
	return m_grassTexture;
}