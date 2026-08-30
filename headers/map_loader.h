#pragma once

#include <fstream>

class MapData;

class SurfaceInfo;
enum class SurfaceType;

class ObjectInstanceData;
class ObjectTypeData;
enum class ObjectType;

class MapLoader
{
public:
	static bool Load(const char* path, MapData& mapData);
private:
	static bool LoadMap(std::ifstream& file, MapData& mapData);
	static bool LoadSurfaceType(std::ifstream& file, SurfaceType& type, SurfaceInfo& data);

	static bool LoadObjects(std::ifstream& file, MapData& mapData);
	static bool LoadObjectType(std::ifstream& file, ObjectType& type, ObjectTypeData& data);
	static bool LoadObject(std::ifstream& file, ObjectInstanceData& objectData);
};