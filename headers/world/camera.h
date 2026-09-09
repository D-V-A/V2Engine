#pragma once

#include "types/vector2f.h"

class Camera
{
public:
	void SetPosition(const Vector2f& position) { m_position = position; }
	Vector2f GetPosition() const { return m_position; }

private:
	Vector2f m_position{};
};