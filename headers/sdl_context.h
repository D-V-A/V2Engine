#pragma once

class SDLContext
{
public:
	SDLContext() = default;
	~SDLContext();

	bool Initialize();

	SDLContext(const SDLContext&) = delete;
	SDLContext& operator=(const SDLContext&) = delete;
};