#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace
{
	constexpr char surfaceTypes[] =
		"SURFACE_TYPES 4\n"
		"G WALKABLE 1.0 txt/grass_tile.png\n"
		"D WALKABLE 0.6 txt/dirt_tile.png\n"
		"W NOT_WALKABLE txt/water_tile.png\n"
		"R WALKABLE 1.4 txt/road_tile.png\n";

	constexpr char objectTypes[] =
		"OBJECT_TYPES 4\n"
		"C 1.0 1.0 COLLISION -1.0 -1.0 1.0 1.0\n"
		"T 1.5 1.5 COLLISION -0.6 -0.6 0.6 0.6\n"
		"R 0.5 0.5 COLLISION -0.5 -0.5 0.5 0.5\n"
		"B 0.75 0.75 NO_COLLISION\n";

	char GenerateSurface(std::mt19937& generator)
	{
		std::uniform_int_distribution<int> distribution(0, 99);

		const int value = distribution(generator);

		if (value < 50)
			return 'G';

		if (value < 70)
			return 'D';

		if (value < 85)
			return 'W';

		return 'R';
	}

	char GenerateObjectType(std::mt19937& generator)
	{
		constexpr char objectSymbols[] = { 'C', 'T', 'R', 'B' };

		std::uniform_int_distribution<int> distribution(0, 3);

		return objectSymbols[distribution(generator)];
	}
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::cout << "Usage: MapGenerator <width> <height> <output> [object_count]\n";
		return 1;
	}

	const int width = std::stoi(argv[1]);
	const int height = std::stoi(argv[2]);
	const std::string outputPath = argv[3];

	int objectCount = 10;

	if (argc >= 5)
		objectCount = std::stoi(argv[4]);

	if (width <= 0 || height <= 0 || objectCount < 0)
	{
		std::cout << "Invalid arguments\n";
		return 1;
	}

	std::ofstream file(outputPath);

	if (!file.is_open())
	{
		std::cout << "Failed to open output file\n";
		return 1;
	}

	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());

	file << surfaceTypes << '\n';

	file << width << ' ' << height << "\n\n";

	std::vector<std::string> map;
	map.reserve(height);

	for (int y = 0; y < height; ++y)
	{
		std::string row;
		row.reserve(width);

		for (int x = 0; x < width; ++x)
			row.push_back(GenerateSurface(generator));

		map.push_back(row);
		file << row << '\n';
	}

	file << '\n';
	file << objectTypes << '\n';

	file << "OBJECTS " << objectCount << '\n';

	std::uniform_real_distribution<float> xDistribution(1.0f, static_cast<float>(width) - 0.01f);
	std::uniform_real_distribution<float> yDistribution(1.0f, static_cast<float>(height) - 0.01f);

	for (int i = 0; i < objectCount; ++i)
	{
		const char type = GenerateObjectType(generator);

		float x = 0.0f;
		float y = 0.0f;

		do
		{
			x = xDistribution(generator);
			y = yDistribution(generator);
		} while (map[static_cast<int>(y)][static_cast<int>(x)] == 'W');

		file << type << ' ' << x << ' ' << y << '\n';
	}

	std::cout << "Map generated: " << outputPath << '\n';

	return 0;
}