#include <cassert>

#include "entities/world_object.h"

#include "world/isometric.h"

#include "graphics/renderer.h"

WorldObject::WorldObject(Vector2f position, Vector2f renderFootprintSize, std::optional<Rect> collisionRect, const std::vector<WorldObjectState>& states)
	: Entity(position, renderFootprintSize),
	m_collisionRect(collisionRect),
	m_states(states)
{}

bool WorldObject::Initialize(Renderer& renderer, const char* texturePath)
{
	return m_texture.Load(renderer, texturePath);
}

bool WorldObject::HasCollision() const
{
	return m_collisionRect.has_value();
}

Rect WorldObject::GetCollisionRect() const
{
	assert(m_collisionRect.has_value());

	Rect result = *m_collisionRect;

	result.x() += m_position.x;
	result.y() += m_position.y;

	return result;
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

bool WorldObject::IsInteractable() const
{
	const WorldObjectState* state = GetCurrentState();

	if (!state)
		return false;

	return state->interaction.has_value();
}

const WorldObjectInteraction* WorldObject::GetInteraction() const
{
	const WorldObjectState* state = GetCurrentState();

	if (!state || !state->interaction)
		return nullptr;

	return &*state->interaction;
}

const WorldObjectState* WorldObject::GetCurrentState() const
{
	if (m_states.empty())
		return nullptr;

	return &m_states[m_currentState];
}

Rect WorldObject::GetInteractionRect() const
{
	if (m_collisionRect.has_value())
		return GetCollisionRect();

	return GetRenderOrderBounds();
}