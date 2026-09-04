#pragma once

#include <SDL3/SDL.h>

#include "types/vector2f.h"

class Window
{
public:
	~Window();

	bool Initialize(const char* title, int width, int height);

	int GetWidth() const;
	int GetHeight() const;
	Vector2f GetCenter() const;

	SDL_Window* GetNativeHandle() const { return m_window; }

private:
	SDL_Window* m_window = nullptr;
};