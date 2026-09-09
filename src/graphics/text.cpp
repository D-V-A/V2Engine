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