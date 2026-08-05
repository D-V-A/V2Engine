#include <SDL3/SDL.h>
#include "renderer.h"
#include "player.h"

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

	SDL_Window* window = nullptr;
	Renderer renderer;
	Player player;

	bool isRunning = false;
};