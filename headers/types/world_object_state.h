#pragma once

#include <optional>
#include <string>

#include "graphics/texture.h"

struct WorldObjectInteraction
{
	std::string text;
	std::optional<size_t> nextState;
};

struct WorldObjectState
{
	std::optional<std::string> texture;
	std::optional<WorldObjectInteraction> interaction;
};

struct WorldObjectRuntimeState
{
	const Texture* texture = nullptr;
	std::optional<WorldObjectInteraction> interaction;
};
