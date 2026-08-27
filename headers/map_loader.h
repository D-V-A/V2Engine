#pragma once

#include <fstream>

class MapData;
class ObjectData;

class MapLoader
{
public:
	static bool Load(const char* path, MapData& mapData);
private:
	static bool LoadMap(std::ifstream& file, MapData& mapData);

	static bool LoadObjects(std::ifstream& file, MapData& mapData);
	static bool LoadObject(std::ifstream& file, ObjectData& objectData);
};