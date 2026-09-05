#include <cassert>

#include "world_object.h"

WorldObject::WorldObject(Vector2f position, Vector2f renderFootprintSize, std::optional<Rect> collisionRect)
	: Entity(position, renderFootprintSize),
	m_collisionRect(collisionRect)
{}

Rect WorldObject::GetCollisionRect() const
{
	assert(m_collisionRect.has_value());

	Rect result = *m_collisionRect;

	result.x() += m_position.x;
	result.y() += m_position.y;

	return result;
}

bool WorldObject::HasCollision() const
{
	return m_collisionRect.has_value();
}

bool WorldObject::Initialize(Renderer& renderer, const char* texturePath)
{
	return InitializeTexture(renderer, m_texture, texturePath);
}