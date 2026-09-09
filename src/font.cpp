#include "font.h"

Font::~Font()
{
	if (m_font)
		TTF_CloseFont(m_font);
}

bool Font::Load(const std::filesystem::path& path, float size)
{
	TTF_Font* font = TTF_OpenFont(path.string().data(), size);

	if (!font)
		return false;

	m_font = font;

	return true;
}

bool TTFContext::Initialize()
{
	return TTF_Init();
}

TTFContext::~TTFContext()
{
	TTF_Quit();
}