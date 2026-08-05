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
	void Update(float deltaTime);
	void Render();

	// Прямоугольник в координатах окна.
	SDL_FRect rectangle{
		440.0f,
		260.0f,
		400.0f,
		200.0f
	};

	SDL_Window* window = nullptr;
	Renderer renderer;

	bool isRunning = false;
};