#pragma once

#include <optional>
#include <memory>
#include <vector>

#include "entities/entity.h"

#include "types/world_object_state.h"
#include "types/rect.h"

struct Vector2f;
class Texture;
class Renderer;

class WorldObject : public Entity
{
public:
	WorldObject(Vector2f position, Vector2f renderFootprintSize, std::optional<Rect> collisionRect,
		std::vector<WorldObjectRuntimeState> states, std::shared_ptr<Texture> baseTexture);
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

	std::shared_ptr<Texture> m_currentTexture;

	std::optional<Rect> m_collisionRect;

	std::vector<WorldObjectRuntimeState> m_states;
	size_t m_currentState = 0;
};