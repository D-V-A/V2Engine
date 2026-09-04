

#include "window.h"


Window::~Window()
{
	if (m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
}

bool Window::Initialize(const char* title, int width, int height)
{
	m_window = SDL_CreateWindow(title, width, height, 0);

	return m_window != nullptr;
}

int Window::GetWidth() const
{
	int width = 0;
	int height = 0;

	SDL_GetWindowSize(m_window, &width, &height);

	return width;
}

int Window::GetHeight() const
{
	int width = 0;
	int height = 0;

	SDL_GetWindowSize(m_window, &width, &height);

	return height;
}

Vector2f Window::GetCenter() const
{
	int width = 0;
	int height = 0;

	SDL_GetWindowSize(m_window, &width, &height);

	return {
		static_cast<float>(width) / 2.0f,
		static_cast<float>(height) / 2.0f
	};
}