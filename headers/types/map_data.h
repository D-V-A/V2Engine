#pragma once

#include <vector>

enum class SurfaceType
{
	Grass,
	Dirt,
	Water
};

struct TileData
{
	SurfaceType surface = SurfaceType::Grass;
};

struct MapData
{
	int width = 0;
	int height = 0;

	std::vector<TileData> tiles;
};