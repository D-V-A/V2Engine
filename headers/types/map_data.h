#pragma once

#include <vector>
#include <map>
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

struct ObjectTypeData
{
	Vector2f renderFootprintSize;
	std::optional<Rect> collision;
};

struct ObjectInstanceData
{
	ObjectType type;
	Vector2f position;
};

struct MapData
{
	int width = 0;
	int height = 0;

	std::vector<TileData> tiles;

	std::map<ObjectType, ObjectTypeData> objectTypes;
	std::vector<ObjectInstanceData> objects;
};