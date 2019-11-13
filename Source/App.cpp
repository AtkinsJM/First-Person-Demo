#include "App.h"
#include "FirstPersonController.h"
#include "PressurePlate.h"

using namespace Leadwerks;

App::App() : window(NULL), context(NULL), world(NULL), camera(NULL)
{

	bUseVSync = false;
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
	camera = Camera::Create();

	Map::Load("Maps/game.map");

	window->HideMouse();

	Model* player = Model::Create();
	player->SetKeyValue("name", "Player");
	Actor* firstPersonController = new FirstPersonController(camera);
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
	
	Entity* pressurePlateObj = world->FindEntity("PressurePlate");
	if (pressurePlateObj)
	{
		Actor* pressurePlate = new PressurePlate();
		pressurePlateObj->SetActor(pressurePlate);
	}
	else
	{
		Print("Pressure plate actor setting failed!");
	}


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