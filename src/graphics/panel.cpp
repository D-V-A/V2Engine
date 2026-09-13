#include "graphics/renderer.h"
#include "graphics/panel.h"

void Panel::Render(Renderer& renderer)const
{
	renderer.FillRect(m_rect, m_color);
}
