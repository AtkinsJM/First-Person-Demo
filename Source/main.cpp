#include "Leadwerks.h"
#include "FirstPersonController.h"
#include "PressurePad.h"

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

	for (int t = 0; t < World::GetCurrent()->CountEntities(); ++t)
	{
		Entity* e = World::GetCurrent()->GetEntity(t);
		if (e->GetKeyValue("name") != "")
		{
			Print(e->GetKeyValue("name"));
		}
		else
		{
			Print("Entity found without name!");
		}
	}
	Entity* pressurePadObj = World::GetCurrent()->FindEntity("Pressure Pad");
	if (pressurePadObj)
	{
		pressurePadObj->SetKeyValue("name", "Pressure Pad");
		Actor* pressurePad = new PressurePad;
		pressurePadObj->SetActor(pressurePad);
	}
	else
	{
		Print("No pressure pad found!");
	}
	Collision::SetResponse(Collision::Trigger, Collision::Prop, Collision::Trigger);
		
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