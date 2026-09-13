#pragma once

#include <string>

#include "ui/primitives/panel.h"
#include "ui/primitives/label.h"

#include "types/vector2f.h"
#include "types/colors.h"

class Font;
class Renderer;
class TextRenderer;

class TextPanel
{
public:
	bool Initialize(TextRenderer& textRenderer, Font& font);

	bool SetText(const std::string& text);
	void SetPosition(const Vector2f& position);
	void SetTextColor(const Color& color);
	void SetBackgroundColor(const Color& color);
	void SetPadding(const Vector2f& padding);
	void SetVisible(bool vis) { m_visible = vis; }

	Vector2f GetSize() const { return m_panel.GetSize(); }

	void Render(Renderer& renderer, TextRenderer& textRenderer) const;

private:
	void UpdateLayout();

	Panel m_panel;
	Label m_label;

	bool m_visible = false;

	Vector2f m_position;
	Vector2f m_padding{ 5.0f, 5.0f };
};