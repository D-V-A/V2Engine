#include "graphics/text_renderer.h"
#include "graphics/renderer.h"
#include "graphics/text.h"

#include "types/vector2f.h"

bool TextRenderer::Initialize(Renderer& renderer)
{
	m_textEngine = TTF_CreateRendererTextEngine(renderer.GetNativeRenderer());

	return m_textEngine != nullptr;
}

TextRenderer::~TextRenderer()
{
	if (m_textEngine)
		TTF_DestroyRendererTextEngine(m_textEngine);
}

bool TextRenderer::DrawText(const Text& text, const Vector2f& position)
{
	return TTF_DrawRendererText(text.GetNativeText(), position.x, position.y);
}