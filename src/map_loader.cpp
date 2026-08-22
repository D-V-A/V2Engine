#include <fstream>
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
}

bool MapLoader::Load(const char* path, MapData& mapData)
{
	std::ifstream file(path);

	if (!file.is_open())
		return false;

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