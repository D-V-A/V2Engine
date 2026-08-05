#include <SDL3/SDL.h>
#include "renderer.h"

class Application
{
public:
	Application() = default;
	~Application();

	int Run();

private:
	bool Initialize();

	void ProcessEvents();
	void Render();

	SDL_Window* window = nullptr;
	Renderer renderer;

	bool isRunning = false;
};