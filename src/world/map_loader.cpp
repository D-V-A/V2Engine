#include <fstream>
#include <string>
#include <utility>

#include <nlohmann/json.hpp>

#include "world/map_loader.h"

#include "types/map_data.h"
#include "types/rect.h"

namespace
{
	using json = nlohmann::json;

	bool IdToSurfaceType(int id, SurfaceType& surfaceType)
	{
		switch (id)
		{
		case 1:
			surfaceType = SurfaceType::Grass;
			return true;

		case 2:
			surfaceType = SurfaceType::Dirt;
			return true;

		case 3:
			surfaceType = SurfaceType::Road;
			return true;

		case 4:
			surfaceType = SurfaceType::Water;
			return true;

		default:
			return false;
		}
	}

	bool IdToObjectType(int id, ObjectType& objectType)
	{
		switch (id)
		{
		case 0:
			objectType = ObjectType::Crate;
			return true;

		case 1:
			objectType = ObjectType::Tree;
			return true;

		case 2:
			objectType = ObjectType::Rock;
			return true;

		case 3:
			objectType = ObjectType::Bush;
			return true;

		default:
			return false;
		}
	}

	bool LoadSurfaceTypes(const json& mapJson, MapData& mapData)
	{
		if (!mapJson.contains("surfaceTypes") || !mapJson["surfaceTypes"].is_array())
			return false;

		for (const auto& surfaceJson : mapJson["surfaceTypes"])
		{
			SurfaceType surfaceType;

			const int id = surfaceJson.at("id").get<int>();

			if (!IdToSurfaceType(id, surfaceType))
				return false;

			SurfaceInfo surfaceInfo;

			surfaceInfo.texture = surfaceJson.at("texture").get<std::string>();
			surfaceInfo.walkable = surfaceJson.at("walkable").get<bool>();

			if (surfaceInfo.walkable)
			{
				if (!surfaceJson.contains("speedModifier"))
					return false;

				surfaceInfo.speedModifier = surfaceJson.at("speedModifier").get<float>();

				if (*surfaceInfo.speedModifier <= 0.0f)
					return false;
			}
			else
			{
				surfaceInfo.speedModifier = std::nullopt;
			}

			const auto [it, inserted] = mapData.surfaceTypes.emplace(surfaceType, std::move(surfaceInfo));

			if (!inserted)
				return false;
		}

		return true;
	}

	bool LoadTiles(const json& mapJson, MapData& mapData)
	{
		if (!mapJson.contains("tiles") || !mapJson["tiles"].is_array())
			return false;

		const auto& tilesJson = mapJson["tiles"];

		const size_t expectedTileCount = static_cast<size_t>(mapData.width) * static_cast<size_t>(mapData.height);

		if (tilesJson.size() != expectedTileCount)
			return false;

		mapData.tiles.reserve(expectedTileCount);

		for (const auto& tileJson : tilesJson)
		{
			SurfaceType surfaceType;

			const int id = tileJson.get<int>();

			if (!IdToSurfaceType(id, surfaceType))
				return false;

			if (!mapData.surfaceTypes.contains(surfaceType))
				return false;

			mapData.tiles.push_back({ surfaceType });
		}

		return true;
	}

	bool LoadObjectTypes(const json& mapJson, MapData& mapData)
	{
		if (!mapJson.contains("objectTypes") || !mapJson["objectTypes"].is_array())
			return false;

		for (const auto& objectJson : mapJson["objectTypes"])
		{
			ObjectType objectType;

			const int id = objectJson.at("id").get<int>();

			if (!IdToObjectType(id, objectType))
				return false;

			ObjectTypeData objectTypeData;

			objectTypeData.texture = objectJson.at("texture").get<std::string>();

			const auto& footprintJson = objectJson.at("renderFootprint");

			if (!footprintJson.is_array() || footprintJson.size() != 2)
				return false;

			objectTypeData.renderFootprintSize = {
				footprintJson[0].get<float>(),
				footprintJson[1].get<float>()
			};

			const bool hasCollisionOffset = objectJson.contains("collisionOffset");
			const bool hasCollisionSize = objectJson.contains("collisionSize");

			if (hasCollisionOffset != hasCollisionSize)
				return false;

			if (hasCollisionOffset)
			{
				const auto& offsetJson = objectJson["collisionOffset"];
				const auto& sizeJson = objectJson["collisionSize"];

				if (!offsetJson.is_array() || offsetJson.size() != 2)
					return false;

				if (!sizeJson.is_array() || sizeJson.size() != 2)
					return false;

				Rect collision;

				collision.position = {
					offsetJson[0].get<float>(),
					offsetJson[1].get<float>()
				};

				collision.size = {
					sizeJson[0].get<float>(),
					sizeJson[1].get<float>()
				};

				if (collision.width() <= 0.0f || collision.height() <= 0.0f)
					return false;

				objectTypeData.collision = collision;
			}
			else
			{
				objectTypeData.collision = std::nullopt;
			}

			if (objectJson.contains("interaction")) 
			{
				InteractionParams inter;

				const auto& interactionJson = objectJson.at("interaction");
				if (!interactionJson.contains("text"))
					return false;
				inter.text = interactionJson.at("text").get<std::string>();

				objectTypeData.interaction = inter;
			}
			else
			{
				objectTypeData.interaction = std::nullopt;
			}

			const auto [it, inserted] = mapData.objectTypes.emplace(objectType, std::move(objectTypeData));

			if (!inserted)
				return false;
		}

		return true;
	}

	bool LoadObjects(const json& mapJson, MapData& mapData)
	{
		if (!mapJson.contains("objects") || !mapJson["objects"].is_array())
			return false;

		const auto& objectsJson = mapJson["objects"];

		mapData.objects.reserve(objectsJson.size());

		for (const auto& objectJson : objectsJson)
		{
			ObjectInstanceData objectData;

			const int typeId = objectJson.at("type").get<int>();

			if (!IdToObjectType(typeId, objectData.type))
				return false;

			if (!mapData.objectTypes.contains(objectData.type))
				return false;

			const auto& positionJson = objectJson.at("position");

			if (!positionJson.is_array() || positionJson.size() != 2)
				return false;

			objectData.position = {
				positionJson[0].get<float>(),
				positionJson[1].get<float>()
			};

			mapData.objects.push_back(objectData);
		}

		return true;
	}
}

bool MapLoader::Load(const std::filesystem::path& path, MapData& mapData)
{
	std::ifstream file(path);

	if (!file.is_open())
		return false;

	try
	{
		json mapJson;
		file >> mapJson;

		MapData loadedMap;

		loadedMap.width = mapJson.at("width").get<int>();
		loadedMap.height = mapJson.at("height").get<int>();

		if (loadedMap.width <= 0 || loadedMap.height <= 0)
			return false;

		if (!LoadSurfaceTypes(mapJson, loadedMap))
			return false;

		if (!LoadTiles(mapJson, loadedMap))
			return false;

		if (!LoadObjectTypes(mapJson, loadedMap))
			return false;

		if (!LoadObjects(mapJson, loadedMap))
			return false;

		mapData = std::move(loadedMap);

		return true;
	}
	catch (const json::exception&)
	{
		return false;
	}
}