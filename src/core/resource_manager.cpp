#include "core/resource_manager.h"

#include "core/assets.h"
#include "graphics/texture.h"

ResourceManager::ResourceManager(Renderer& renderer)
	: m_renderer(renderer)
{}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::filesystem::path& path)
{
	auto it = m_textures.find(path);

	if (it != m_textures.end())
	{
		if (auto texture = it->second.lock())
			return texture;
	}

	auto texture = std::make_shared<Texture>();

	const std::filesystem::path assetPath = GetAssetPath(path);

	if (!texture->Load(m_renderer, assetPath.string().c_str()))
		return nullptr;

	m_textures[path] = texture;

	return texture;		
}