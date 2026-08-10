#pragma once

struct SDL_Texture;
class Renderer;

class Texture
{
public:
	Texture() = default;
	~Texture();

	bool Load(Renderer& renderer, const char* path);

	int GetWidth() const;
	int GetHeight() const;

	SDL_Texture* GetNativeTexture() const;

private:
	SDL_Texture* sdlTexture = nullptr;

	int width = 0;
	int height = 0;
};