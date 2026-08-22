
#include <string>
#include <utility>

#include "map_loader.h"
#include "types/map_data.h"


namespace
{
	bool CharToSurfaceType(char symbol, SurfaceType& surfaceType)
	{
		switch (symbol)
		{
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

	bool CharToObjectType(char symbol, ObjectType& surfaceType)
	{
		switch (symbol)
		{
		case 'C':
			surfaceType = ObjectType::Crate;
			return true;

		case 'T':
			surfaceType = ObjectType::Tree;
			return true;

		case 'R':
			surfaceType = ObjectType::Rock;
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

	if(!LoadMap(file, mapData))
		return false;

	if (!LoadObjects(file, mapData))
		return false;

	return true;
}

bool MapLoader::LoadMap(std::ifstream& file, MapData& mapData)
{
	int width = 0;
	int height = 0;

	if (!(file >> width >> height))
		return false;

	if (width <= 0 || height <= 0)
		return false;

	MapData loadedMap;
	loadedMap.width = width;
	loadedMap.height = height;
	loadedMap.tiles.reserve(static_cast<size_t>(width) * height);

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

			if (!CharToSurfaceType(symbol, surfaceType))
				return false;

			loadedMap.tiles.push_back({ surfaceType });
		}
	}

	mapData = std::move(loadedMap);
	return true;
}

bool MapLoader::LoadObjects(std::ifstream& file, MapData& mapData)
{
	std::string objectsHeader;
	int objectCount = 0;

	if (!(file >> objectsHeader >> objectCount))
		return false;

	if (objectsHeader != "OBJECTS" || objectCount < 0)
		return false;

	mapData.objects.reserve(objectCount);

	bool sucessful = true;
	for (int i = 0; i < objectCount; i++)
	{
		char type;
		ObjectData obj;
		if (!(file >> type >> obj.position.x >> obj.position.y >> obj.renderFootprintSize.x >> obj.renderFootprintSize.y) ||
			!CharToObjectType(type, obj.type))
		{
			sucessful = false;
			continue;
		}

		mapData.objects.push_back(obj);
	}
	if (!sucessful)
	{
		mapData.objects.shrink_to_fit();
		return false;
	}
	return true;
}