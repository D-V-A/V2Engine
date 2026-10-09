#pragma once

#include <filesystem>
#include <map>
#include <memory>

class Renderer;
class Texture;

class ResourceManager
{
public:
	ResourceManager(Renderer& renderer);

	std::shared_ptr<Texture> GetTexture(const std::filesystem::path& path);

private:
	Renderer& m_renderer;
	std::map<std::filesystem::path, std::weak_ptr<Texture>> m_textures;
};