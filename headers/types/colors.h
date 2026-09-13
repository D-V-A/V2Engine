#pragma once

#include <SDL3/SDL_stdinc.h>

#include <array>

struct Color
{
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 a = 255;
};

enum class Colors
{
	White,
	Black,
	Red,
	Green,
	Blue,
	Yellow,
	Gray,
	DarkGray,

	Count
};


inline constexpr std::array<Color, static_cast<size_t>(Colors::Count)> ColorPalette{
	Color{ 255, 255, 255, 255 },	// White
	Color{ 0, 0, 0, 255 },			// Black
	Color{ 255, 0, 0, 255 },		// Red
	Color{ 0, 255, 0, 255 },		// Green
	Color{ 0, 0, 255, 255 },		// Blue
	Color{ 255, 255, 0, 255 },		// Yellow
	Color{ 128, 128, 128, 255 },	// Gray
	Color{ 64, 64, 64, 255 }		// DarkGray
};

inline constexpr const Color& GetColor(Colors color)
{
	return ColorPalette[static_cast<size_t>(color)];
}