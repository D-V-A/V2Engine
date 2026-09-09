#pragma once

#include <SDL3_ttf/SDL_textengine.h>

class Renderer;
class Vector2f;
class Text;

class TextRenderer
{
public:
	TextRenderer() = default;
	~TextRenderer();

	bool Initialize(Renderer& renderer);

	TextRenderer(const TextRenderer&) = delete;
	TextRenderer& operator=(const TextRenderer&) = delete;

public:
	bool DrawText(const Text& text, const Vector2f& position);

	TTF_TextEngine* GetNativeTextEngine() const { return m_textEngine; };

private:
	TTF_TextEngine* m_textEngine = nullptr;
};