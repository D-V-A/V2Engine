#pragma once

#include "texture.h"

class Renderer;

class World
{
public:
	bool Initialize(Renderer& renderer);
	void Render(Renderer& renderer) const;

private:
	Texture floorTexture;

	int width = 10;
	int height = 10;
};