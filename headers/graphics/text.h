#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include <string>

#include "types/vector2f.h"
#include "types/colors.h"

class Font;
class TextRenderer;

class Text
{
public:
	Text() = default;
	~Text();

	bool Initialize(TextRenderer& textRenderer, Font& font, const std::string& text);

	Text(const Text&) = delete;
	Text& operator=(const Text&) = delete;


public:
	bool SetText(const std::string& text);
	bool SetColor(const Color& color);

	TTF_Text* GetNativeText() const { return m_text; }
	Vector2f GetSize() const;

private:
	TTF_Text* m_text = nullptr;
};