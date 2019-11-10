#include "Leadwerks.h"
#include "FirstPersonController.h"

using namespace Leadwerks;

int main(int argc, const char* argv[])
{
	Window* gameWindow = Window::Create("My Game", 0, 0, 1024, 768);
	Context* context = Context::Create(gameWindow);
	World* world = World::Create();

	bool bUseVSync = false;

	Map::Load("Maps/game.map");

	gameWindow->HideMouse();
			
	Model* player = Model::Create();
	player->SetKeyValue("name", "Player");
	Actor* firstPersonController = new FirstPersonController;
	player->SetActor(firstPersonController);
		
	while (true)
	{		
		if (gameWindow->Closed() || gameWindow->KeyHit(Key::Escape)) { return false; }
			
		Time::Update();
		world->Update();
		world->Render();
		context->Sync(bUseVSync);
	}
	return 0;
}