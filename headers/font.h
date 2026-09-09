#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include <filesystem>
#include <string>

class Font
{
public:
	Font() = default;
	~Font();

	bool Load(const std::filesystem::path& path, float size);

	Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;

	TTF_Font* GetNativeFont() const { return m_font; };

private:
	TTF_Font* m_font = nullptr;
};

class TTFContext
{
public:
	TTFContext() = default;
	~TTFContext();

	bool Initialize();

	TTFContext(const TTFContext&) = delete;
	TTFContext& operator=(const TTFContext&) = delete;
};