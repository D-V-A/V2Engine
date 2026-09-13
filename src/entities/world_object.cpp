#include <cassert>

#include "entities/world_object.h"

#include "world/isometric.h"

#include "graphics/renderer.h"
#include "graphics/texture.h"

WorldObject::WorldObject(Vector2f position, Vector2f renderFootprintSize, std::optional<Rect> collisionRect, 
						const std::vector<WorldObjectRuntimeState>& states, const Texture& baseTexture)
	: Entity(position, renderFootprintSize),
	m_collisionRect(collisionRect),
	m_states(states),
	m_currentTexture(&baseTexture)	
{
	if (!m_states.empty() && m_states[0].texture)
		m_currentTexture = m_states[0].texture;
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
	assert(m_currentTexture);

	Rect renderRect;
	renderRect.size = {
		static_cast<float>(m_currentTexture->GetWidth()),
		static_cast<float>(m_currentTexture->GetHeight())
	};

	renderRect.position = GetTopLeft(screenPosition, renderRect.size, m_pivot);

	renderer.DrawTexture(*m_currentTexture, renderRect);
}

bool WorldObject::Interact()
{
	const WorldObjectInteraction* interaction = GetInteraction();

	if (!interaction || !interaction->nextState)
		return false;

	if (*interaction->nextState >= m_states.size())
		return false;

	m_currentState = *interaction->nextState;
	m_currentTexture = m_states[m_currentState].texture;

	return true;
}

bool WorldObject::IsInteractable() const
{
	const WorldObjectRuntimeState* state = GetCurrentState();

	if (!state)
		return false;

	return state->interaction.has_value();
}

const WorldObjectInteraction* WorldObject::GetInteraction() const
{
	const WorldObjectRuntimeState* state = GetCurrentState();

	if (!state || !state->interaction)
		return nullptr;

	return &*state->interaction;
}

const WorldObjectRuntimeState* WorldObject::GetCurrentState() const
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