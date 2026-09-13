#pragma once

#include <string>

#include "graphics/text_renderer.h"
#include "graphics/font.h"
#include "graphics/text.h"

#include "types/vector2f.h"
#include "types/colors.h"

class Label
{
public:
	bool Initialize(TextRenderer& textRenderer, Font& font) { return m_text.Initialize(textRenderer, font, ""); }

	bool SetText(const std::string& text) { return m_text.SetText(text); }
	bool SetColor(const Color& color) { return m_text.SetColor(color); }

	void SetPosition(const Vector2f& position) { m_position = position; }

	Vector2f GetPosition() const { return m_position; }
	Vector2f GetSize() const { return m_text.GetSize(); }
	
	void Render(TextRenderer& textRenderer) const { textRenderer.DrawText(m_text, m_position); };

private:
	Text m_text;
	Vector2f m_position;
};