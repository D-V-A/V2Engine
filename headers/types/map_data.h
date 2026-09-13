#pragma once

#include <vector>
#include <map>
#include <optional>
#include <string>

#include "types/vector2f.h"
#include "types/rect.h"

//MapTiles
enum class SurfaceType
{
	Void	= 0,
	Grass	= 1,
	Dirt	= 2,
	Road	= 3,
	Water	= 4,
	Count
};

struct TileData
{
	SurfaceType surface = SurfaceType::Void;
};

struct SurfaceInfo
{
	bool walkable = true;
	std::optional<float> speedModifier;
	std::string texture;
};

//Objects
enum class ObjectType
{
	Unknown,
	Crate,
	Tree,
	Rock,
	Bush
};

struct InteractionParams
{
	std::string text;
};

struct ObjectTypeData
{
	Vector2f renderFootprintSize{};
	std::string texture;
	std::optional<Rect> collision;
	std::optional<InteractionParams> interaction;
};

struct ObjectInstanceData
{
	ObjectType type = ObjectType::Unknown;
	Vector2f position{};
};

struct MapData
{
	int width = 0;
	int height = 0;

	std::map<SurfaceType, SurfaceInfo> surfaceTypes;
	std::vector<TileData> tiles;

	std::map<ObjectType, ObjectTypeData> objectTypes;
	std::vector<ObjectInstanceData> objects;
};