#pragma once

#include <vector>

class Entity;

std::vector<const Entity*> BuildRenderOrder(const std::vector<const Entity*>& entities);