#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include <string>

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

	TTF_Text* GetNativeText() const { return m_text; }

private:
	TTF_Text* m_text = nullptr;
};