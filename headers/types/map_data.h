#pragma once

#include <vector>
#include <optional>

#include "types/Vector2f.h"
#include "types/rect.h"

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
	Rock,
	Bush
};

struct TileData
{
	SurfaceType surface = SurfaceType::Grass;
};

struct ObjectData
{
	ObjectType type;
	Vector2f position; //world coordinates

	Vector2f renderFootprintSize;

	std::optional<Rect> collision;
};

struct MapData
{
	int width = 0;
	int height = 0;

	std::vector<TileData> tiles;
	std::vector<ObjectData> objects;
};