#include <fstream>

class MapData;

class MapLoader
{
public:
	static bool Load(const char* path, MapData& mapData);
private:
	static bool LoadMap(std::ifstream& file, MapData& mapData);
	static bool LoadObjects(std::ifstream& file, MapData& mapData);
};