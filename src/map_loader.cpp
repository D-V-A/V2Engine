
#include <string>
#include <utility>

#include "map_loader.h"
#include "types/map_data.h"
#include "types/rect.h"

namespace
{
	bool CharToSurfaceType(char symbol, SurfaceType& surfaceType)
	{
		switch (symbol)
		{
		case 'R':
			surfaceType = SurfaceType::Road;
			return true;

		case 'G':
			surfaceType = SurfaceType::Grass;
			return true;

		case 'D':
			surfaceType = SurfaceType::Dirt;
			return true;

		case 'W':
			surfaceType = SurfaceType::Water;
			return true;

		default:
			return false;
		}
	}

	bool CharToObjectType(char symbol, ObjectType& objectType) 
	{
		switch (symbol)
		{
		case 'C':
			objectType = ObjectType::Crate;
			return true;

		case 'T':
			objectType = ObjectType::Tree;
			return true;

		case 'R':
			objectType = ObjectType::Rock;
			return true;

		case 'B':
			objectType = ObjectType::Bush;
			return true;

		default:
			return false;
		}
	}
}

bool MapLoader::Load(const char* path, MapData& mapData)
{
	std::ifstream file(path);

	if (!file.is_open())
		return false;

	MapData loadedMap;

	if(!LoadMap(file, loadedMap))
		return false;

	if (!LoadObjects(file, loadedMap))
		return false;

	mapData = std::move(loadedMap);

	return true;
}

bool MapLoader::LoadMap(std::ifstream& file, MapData& mapData)
{
	std::string header;
	int typesCount = 0;

	if (!(file >> header >> typesCount))
		return false;

	if (header != "SURFACE_TYPES" || typesCount <= 0)
		return false;

	for (int i = 0; i < typesCount; i++)
	{
		SurfaceType surfaceType;
		SurfaceInfo surfData;
		if (!LoadSurfaceType(file, surfaceType, surfData))
			return false;

		const auto [it, inserted] = mapData.surfaceTypes.emplace(surfaceType, std::move(surfData));

		if (!inserted)
			return false;
	}

	int width = 0;
	int height = 0;

	if (!(file >> width >> height))
		return false;

	if (width <= 0 || height <= 0)
		return false;

	mapData.width = width;
	mapData.height = height;
	mapData.tiles.reserve(static_cast<size_t>(width) * height);

	std::string row;

	for (int y = 0; y < height; ++y)
	{
		if (!(file >> row))
			return false;

		if (row.size() != static_cast<size_t>(width))
			return false;

		for (char symbol : row)
		{
			SurfaceType surfaceType;

			if (!CharToSurfaceType(symbol, surfaceType) || !mapData.surfaceTypes.contains(surfaceType))
				return false;

			mapData.tiles.push_back({ surfaceType });
		}
	}

	return true;
}

bool MapLoader::LoadSurfaceType(std::ifstream& file, SurfaceType& type, SurfaceInfo& data)
{
	char typeSymbol;

	if (!(file >> typeSymbol))
		return false;

	if (!CharToSurfaceType(typeSymbol, type))
		return false;

	std::string walkable;
	if (!(file >> walkable))
		return false;

	if (walkable == "NOT_WALKABLE")
	{
		data.walkable = false;
		data.speedModifier = std::nullopt;
	}
	else if(walkable == "WALKABLE")
	{
		float modifier;
		
		if (!(file >> modifier) || modifier <= 0.0f)
			return false;
		
		data.walkable = true;
		data.speedModifier = modifier;
	}
	else
		return false;

	std::string texturePath;
	if (!(file >> texturePath))
		return false;

	data.texture = std::move(texturePath);

	return true;
}

bool MapLoader::LoadObjects(std::ifstream& file, MapData& mapData)
{
	std::string header;
	int typesCount = 0;

	if (!(file >> header >> typesCount))
		return false;

	if (header != "OBJECT_TYPES" || typesCount < 0)
		return false;

	for (int i = 0; i < typesCount; i++)
	{
		ObjectType type;
		ObjectTypeData typeData;

		if(!LoadObjectType(file, type, typeData))
			return false;

		const auto [it, inserted] = mapData.objectTypes.emplace(type, std::move(typeData));

		if (!inserted)
			return false;
	}

	int objectCount = 0;

	if (!(file >> header >> objectCount))
		return false;

	if (header != "OBJECTS" || objectCount < 0)
		return false;

	mapData.objects.reserve(objectCount);

	for (int i = 0; i < objectCount; i++)
	{
		ObjectInstanceData obj;
		
		if (!LoadObject(file, obj) || !mapData.objectTypes.contains(obj.type))
			return false;

		mapData.objects.push_back(obj);
	}
	return true;
}

bool MapLoader::LoadObjectType(std::ifstream& file, ObjectType& type, ObjectTypeData& data)
{
	char typeSymbol;

	if (!(file >> typeSymbol >> data.renderFootprintSize.x >> data.renderFootprintSize.y))
		return false;

	if (!CharToObjectType(typeSymbol, type))
		return false;

	std::string collisionType;

	if (!(file >> collisionType))
		return false;

	if (collisionType == "NO_COLLISION")
	{
		data.collision = std::nullopt;
		return true;
	}

	if (collisionType != "COLLISION")
		return false;

	Rect collision;

	if (!(file >> collision.x() >> collision.y() >> collision.width() >> collision.height()))
		return false;

	if (collision.width() <= 0.0f || collision.height() <= 0.0f)
		return false;

	data.collision = collision;

	return true;
}

bool MapLoader::LoadObject(std::ifstream& file, ObjectInstanceData& objectData)
{
	char type;

	if (!(file >> type >> objectData.position.x >> objectData.position.y))
		return false;

	if (!CharToObjectType(type, objectData.type))
		return false;

	return true;
}