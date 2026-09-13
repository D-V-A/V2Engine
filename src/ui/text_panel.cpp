#include "ui/text_panel.h"

#include "graphics/font.h"
#include "graphics/renderer.h"
#include "graphics/text_renderer.h"

bool TextPanel::Initialize(TextRenderer& textRenderer, Font& font)
{
	if (!m_label.Initialize(textRenderer, font))
		return false;

	UpdateLayout();

	return true;
}

bool TextPanel::SetText(const std::string& text)
{
	if (!m_label.SetText(text))
		return false;

	UpdateLayout();

	return true;
}

void TextPanel::SetPosition(const Vector2f& position)
{
	m_position = position;
	UpdateLayout();
}

void TextPanel::SetPadding(const Vector2f& padding)
{
	m_padding = padding;
	UpdateLayout();
}

void TextPanel::SetTextColor(const Color& color)
{
	m_label.SetColor(color);
}

void TextPanel::SetBackgroundColor(const Color& color)
{
	m_panel.SetColor(color);
}

void TextPanel::Render(Renderer& renderer, TextRenderer& textRenderer) const
{
	if (!m_visible)
		return;

	m_panel.Render(renderer);
	m_label.Render(textRenderer);
}

void TextPanel::UpdateLayout()
{
	m_panel.SetPosition(m_position);

	m_label.SetPosition({
		m_position.x + m_padding.x,
		m_position.y + m_padding.y
		});

	const Vector2f textSize = m_label.GetSize();

	m_panel.SetSize({
		textSize.x + m_padding.x * 2.0f,
		textSize.y + m_padding.y * 2.0f
		});
}