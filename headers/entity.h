#pragma once

#include "texture.h"

#include "types/vector2f.h"
#include "types/rect.h"

class Renderer;

class Entity
{
public:
	Entity(Vector2f pos);
	Entity(Vector2f pos, Vector2f pivot);
	virtual ~Entity() = default;

	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	Entity(Entity&&) noexcept = default;
	Entity& operator=(Entity&&) noexcept = default;

public:
	const Vector2f& GetPosition() const { return m_position; }
	void SetPosition(const Vector2f& newPosition) { m_position = newPosition; }

	float GetDepth() const { return m_position.x + m_position.y; }

	virtual bool Initialize(Renderer& renderer, const char* texturePath);
	virtual void Render(Renderer& renderer, const Vector2f& screenPosition) const;

protected:
	Vector2f m_position{ -1.0f,-1.0f };//in world coordinates
	Vector2f m_pivot{ 0.5f, 1.0f };

	Texture m_texture;
	Rect m_textureRectangle;
};