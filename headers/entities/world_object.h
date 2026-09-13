#pragma once

#include <optional>
#include <string>
#include <vector>

#include "graphics/texture.h"

#include "entities/entity.h"

#include "types/world_object_state.h"

class WorldObject : public Entity
{
public:
	WorldObject(Vector2f position, Vector2f renderFootprintSize, std::optional<Rect> collisionRect, const std::vector<WorldObjectState>& states);

	bool Initialize(Renderer& renderer, const char* texturePath);

public:
	bool HasCollision() const;
	Rect GetCollisionRect() const;

	bool IsInteractable() const;
	const WorldObjectInteraction* GetInteraction() const;
	Rect GetInteractionRect() const;

	void Render(Renderer& renderer, const Vector2f& screenPosition) const override;

private:
	const WorldObjectState* GetCurrentState() const;

private:
	Texture m_texture;

	std::optional<Rect> m_collisionRect;

	std::vector<WorldObjectState> m_states;
	size_t m_currentState = 0;
};