#include <SDL3/SDL.h>

#include "renderer.h"
#include "world.h"
#include "player.h"
#include "world_object.h"
#include "input.h"

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
	World world;
	Player player;
	WorldObject worldObject;
	Input input;

	bool isRunning = false;
};