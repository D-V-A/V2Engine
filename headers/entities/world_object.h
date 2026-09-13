#pragma once

#include <optional>
#include <string>
#include <vector>

#include "entities/entity.h"

#include "types/world_object_state.h"

class Vector2f;
class Texture;

class WorldObject : public Entity
{
public:
	WorldObject(Vector2f position, Vector2f renderFootprintSize, std::optional<Rect> collisionRect,
		const std::vector<WorldObjectRuntimeState>& states, const Texture& baseTexture);
public:
	bool HasCollision() const;
	Rect GetCollisionRect() const;

	bool IsInteractable() const;
	const WorldObjectInteraction* GetInteraction() const;
	Rect GetInteractionRect() const;

	bool Interact();

	void Render(Renderer& renderer, const Vector2f& screenPosition) const override;

private:
	const WorldObjectRuntimeState* GetCurrentState() const;

	const Texture* m_currentTexture = nullptr;

	std::optional<Rect> m_collisionRect;

	std::vector<WorldObjectRuntimeState> m_states;
	size_t m_currentState = 0;
};