#pragma once

#include <filesystem>

class MapData;

class MapLoader
{
public:
	static bool Load(const std::filesystem::path& path, MapData& mapData);
};