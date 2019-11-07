#include "Leadwerks.h"

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
	player->SetPhysicsMode(Entity::CharacterPhysics);
	player->SetPosition(0, 2, 0);
	player->SetMass(1);
	
	//Model* playerBox = Model::Box(player);
	//playerBox->SetPosition(0, (playerBox->GetScale().y) / 2, 0);

	Camera* mainCamera = Camera::Create(player);

	mainCamera->SetRotation(0, 0, 0);
	mainCamera->SetPosition(0, 1.8f, 0);
	//mainCamera->SetDebugPhysicsMode(true);

	float movementSpeed = 5;
	float jumpSpeed = 10;
	float turnRate = 0.15f;
	float lookUpRate = 0.1f;
	float lookUpAngleClamp = 75.0f;

	Vec2 screenCentre = Vec2(gameWindow->GetWidth() / 2, gameWindow->GetHeight() / 2);
	
	while (true)
	{
		
		if (gameWindow->Closed() || gameWindow->KeyHit(Key::Escape)) { return false; }

		Vec3 mouseDelta = gameWindow->GetMousePosition() - Vec3(screenCentre.x, screenCentre.y);
		gameWindow->SetMousePosition(screenCentre.x, screenCentre.y);
		
		Vec2 movementInput = Vec2(gameWindow->KeyDown(Key::W) - gameWindow->KeyDown(Key::S), gameWindow->KeyDown(Key::D) - gameWindow->KeyDown(Key::A)).Normalize();
		
		float forward = movementInput.x * movementSpeed;
		float right = movementInput.y * movementSpeed;
		
		float jump = gameWindow->KeyHit(Key::Space) * jumpSpeed;
		
		float turnRotation = player->GetRotation().y + (mouseDelta.x * turnRate);
		float lookUpRotation = Math::Clamp(mainCamera->GetRotation().x + (mouseDelta.y * lookUpRate), -lookUpAngleClamp, lookUpAngleClamp);
		
		player->SetInput(turnRotation, forward, right, jump);
		mainCamera->SetRotation(lookUpRotation, 0, 0);

		Time::Update();
		world->Update();
		world->Render();
		context->Sync(bUseVSync);
	}
	return 0;
}