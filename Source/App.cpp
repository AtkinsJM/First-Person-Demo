#include "App.h"
#include "FirstPersonController.h"
#include "PressurePad.h"

using namespace Leadwerks;

App::App() : window(NULL), context(NULL), world(NULL)
{

	bUseVSync = false;
	//camera = nullptr;
}

App::~App()
{
	delete world;
	delete window;
}

bool App::Start()
{
	window = Window::Create("My Game", 0, 0, 1024, 768);
	context = Context::Create(window);
	world = World::Create();
	
	Map::Load("Maps/game.map");

	window->HideMouse();

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

	//Collision::SetResponse(Collision::Trigger, Collision::Prop, Collision::Trigger);

	return true;
}

bool App::Loop()
{
	if (window->Closed() || window->KeyHit(Key::Escape)) { return false; }

	Time::Update();
	world->Update();
	world->Render();
	context->Sync(bUseVSync);
	return true;
}