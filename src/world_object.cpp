#include <cassert>

#include "world_object.h"
#include "isometric.h"
#include "renderer.h"

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
	return m_texture.Load(renderer, texturePath);
}

void WorldObject::Render(Renderer& renderer, const Vector2f& screenPosition) const
{
	const Texture& texture = m_texture;

	Rect renderRect;
	renderRect.size = {
		static_cast<float>(texture.GetWidth()),
		static_cast<float>(texture.GetHeight())
	};

	renderRect.position = GetTopLeft(screenPosition, renderRect.size, m_pivot);

	renderer.DrawTexture(texture, renderRect);
}