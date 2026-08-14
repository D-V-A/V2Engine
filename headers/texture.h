#pragma once

struct SDL_Texture;
class Renderer;

class Texture
{
public:
	Texture() = default;
	~Texture();

	bool Load(Renderer& renderer, const char* path);

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	SDL_Texture* GetNativeTexture() const;

private:
	SDL_Texture* m_sdlTexture = nullptr;

	int m_width = 0;
	int m_height = 0;
};