#pragma once 

#include "types/vector2f.h"
#include "types/colors.h"
#include "types/rect.h"

class Renderer;

class Panel
{
public:
	void SetPosition(const Vector2f& position) { m_rect.position = position; }
	void SetSize(const Vector2f& size) { m_rect.size = size; }
	void SetColor(const Color& color) { m_color = color; }

	const Rect& GetRect() const { return m_rect; }

	void Render(Renderer& renderer) const;

private:
	Rect m_rect;
	Color m_color = GetColor(Colors::DarkGray);
};