#pragma once

#include <utility>

class Input
{
public:
	void Update();

	std::pair<int, int> GetDirection() { return movementDirection; };

private:
	std::pair<int, int> movementDirection{ 0,0 };
	const bool* keyboardState = nullptr;
};