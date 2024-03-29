#include "App.h"
#include "FirstPersonController.h"
#include "PressurePlate.h"
#include "MovingPlatform.h"
#include "Elevator.h"

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
	window = Window::Create("My Game", 0, 0, 1280, 720);
	context = Context::Create(window);
	world = World::Create();
	camera = Camera::Create();

	Map::Load("Maps/game.map");

	window->HideMouse();

	Model* player = Model::Create();
	player->SetKeyValue("name", "Player");
	Actor* firstPersonController = new FirstPersonController(camera);
	player->SetActor(firstPersonController);


	for (int t = 0; t < world->CountEntities(); ++t)
	{
		Entity* e = world->GetEntity(t);
		if (e->GetKeyValue("name") != "")
		{
			Print(e->GetKeyValue("name"));
		}
		else
		{
			Print("Entity found without name!");
		}
	}

	for (int i = 0; i < world->CountEntities(); i++)
	{
		Entity* e = world->GetEntity(i);
		if (e->GetKeyValue("tag") == "PressurePlate")
		{
			Print("Found a pressure plate! Setting up now...");
			Actor* pressurePlate = new PressurePlate();
			e->SetActor(pressurePlate);
		}
		else if (e->GetKeyValue("name") == "crate1")
		{
			Print("Found a crate!");
		}
		else if (e->GetKeyValue("tag") == "MovingPlatform")
		{
			Actor* movingPlatform = new MovingPlatform();
			e->SetActor(movingPlatform);
		}
		else if (e->GetKeyValue("tag") == "Elevator")
		{
			Actor* elevator = new Elevator();
			e->SetActor(elevator);
		}
	}

	/*
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
	*/

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