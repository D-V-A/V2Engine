#pragma once

#include <optional>

#include "entity.h"

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
	bool InitializeTexture(Renderer& renderer, Texture& texture, const char* texturePath);
	const Texture& GetCurrentTexture() const { return m_texture; };

	Texture m_texture;

	std::optional<Rect> m_collisionRect;
};