#include "assets.h"

std::filesystem::path GetAssetPath(const std::filesystem::path& relativePath)
{
	return (std::filesystem::path(V2ENGINE_ASSET_ROOT) / relativePath).make_preferred();
}