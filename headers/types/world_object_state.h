#pragma once

#include <optional>
#include <string>

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