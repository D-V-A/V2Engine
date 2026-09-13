#include "graphics/text_renderer.h"
#include "graphics/text.h"
#include "graphics/font.h"

Text::~Text()
{
	if (m_text)
		TTF_DestroyText(m_text);
}

bool Text::Initialize(TextRenderer& textRenderer, Font& font, const std::string& text)
{
	TTF_Text* newText = TTF_CreateText(textRenderer.GetNativeTextEngine(), font.GetNativeFont(), text.c_str(), 0);

	if (!newText)
		return false;

	m_text = newText;

	return true;
}

bool Text::SetText(const std::string& text)
{
	return TTF_SetTextString(m_text, text.c_str(), 0);
}

bool Text::SetColor(const Color& color)
{
	return TTF_SetTextColor(m_text, color.r, color.g, color.b, color.a);
}

Vector2f Text::GetSize() const
{
	int width = 0;
	int height = 0;

	if (!TTF_GetTextSize(m_text, &width, &height))
		return {};

	return { static_cast<float>(width), static_cast<float>(height) };
}