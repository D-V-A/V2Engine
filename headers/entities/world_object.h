#pragma once

#include <optional>

#include "graphics/texture.h"

#include "entities/entity.h"

class WorldObject: public Entity
{
public:
	WorldObject(Vector2f position, Vector2f renderFootprintSize, std::optional<Rect> collisionRect);

	bool Initialize(Renderer& renderer, const char* texturePath);

public:
	bool HasCollision() const;
	Rect GetCollisionRect() const;

	void Render(Renderer& renderer, const Vector2f& screenPosition) const override;

private:
	Texture m_texture;

	std::optional<Rect> m_collisionRect;
};