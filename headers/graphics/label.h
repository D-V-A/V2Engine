#pragma once

#include <string>

#include "graphics/text.h"
#include "types/vector2f.h"
#include "types/colors.h"

class Font;
class TextRenderer;

class Label
{
public:
	bool Initialize(TextRenderer& textRenderer, Font& font, const std::string& text);
	bool Initialize(TextRenderer& textRenderer, Font& font);

	bool SetText(const std::string& text);
	bool SetColor(const Color& color);

	void SetPosition(const Vector2f& position) { m_position = position; }

	Vector2f GetPosition() const { return m_position; }
	Vector2f GetSize() const { return m_text.GetSize(); }

	void Render(TextRenderer& textRenderer) const;

private:
	Text m_text;
	Vector2f m_position;
};