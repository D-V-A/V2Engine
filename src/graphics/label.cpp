#include "graphics/font.h"
#include "graphics/label.h"
#include "graphics/text_renderer.h"

bool Label::Initialize(TextRenderer& textRenderer, Font& font, const std::string& text)
{
	return m_text.Initialize(textRenderer, font, text);
}

bool Label::Initialize(TextRenderer& textRenderer, Font& font)
{
	return m_text.Initialize(textRenderer, font, "");
}

bool Label::SetText(const std::string& text)
{
	return m_text.SetText(text);
}

bool Label::SetColor(const Color& color)
{
	return m_text.SetColor(color);
}

void Label::Render(TextRenderer& textRenderer) const
{
	textRenderer.DrawText(m_text, m_position);
}