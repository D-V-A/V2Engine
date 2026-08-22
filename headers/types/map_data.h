#pragma once

#include <vector>

#include "types/Vector2f.h"

enum class SurfaceType
{
	Grass,
	Dirt,
	Water
};

enum class ObjectType
{
	Crate,
	Tree,
	Rock
};

struct TileData
{
	SurfaceType surface = SurfaceType::Grass;
};

struct ObjectData
{
	ObjectType type;
	Vector2f position;
	Vector2f renderFootprintSize;
};

struct MapData
{
	int width = 0;
	int height = 0;

	std::vector<TileData> tiles;
	std::vector<ObjectData> objects;
};