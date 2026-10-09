#pragma once

#include <optional>
#include <memory>
#include <string>

class Texture;

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
	std::shared_ptr<Texture> texture;
	std::optional<WorldObjectInteraction> interaction;
};
