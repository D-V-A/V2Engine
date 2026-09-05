#pragma once

#include "texture.h"

#include "types/vector2f.h"
#include "types/rect.h"

class Renderer;

class Entity
{
public:
	Entity(Vector2f pos);
	Entity(Vector2f pos, Vector2f renderFootprintSize);
	virtual ~Entity() = default;

	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	Entity(Entity&&) noexcept = default;
	Entity& operator=(Entity&&) noexcept = default;

public:
	const Vector2f& GetPosition() const { return m_position; }
	void SetPosition(const Vector2f& newPosition) { m_position = newPosition; }

	bool InitializeTexture(Renderer& renderer, Texture& texture, const char* texturePath);
	virtual void Render(Renderer& renderer, const Vector2f& screenPosition) const;

	Rect GetRenderOrderBounds() const;

protected:
	virtual const Texture& GetCurrentTexture() const { return m_texture; }

	Vector2f m_position{ -1.0f,-1.0f };//in world coordinates
	Vector2f m_pivot{ 0.5f, 1.0f };

	Rect m_renderOrderBounds;

	Texture m_texture;
};