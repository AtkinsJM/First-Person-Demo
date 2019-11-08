#include "FirstPersonController.h"

FirstPersonController::FirstPersonController()
{
	gameWindow = Window::GetCurrent();
	screenCentre = Vec2(gameWindow->GetWidth() / 2, gameWindow->GetHeight() / 2);

	walkingSpeed = 4;
	jumpSpeed = 7;
	turnRate = 0.15f;
	lookUpRate = 0.1f;
	lookUpAngleClamp = 75.0f;

	standingHeight = 1.8f;
	crouchingHeight = 0.9f;

	crouchingSpeedModifier = 0.5f;
	sprintingSpeedModifier = 2.0f;
}

FirstPersonController::~FirstPersonController()
{
}

void FirstPersonController::Attach()
{
	mainCamera = Camera::Create(entity);
	mainCamera->SetRotation(0, 0, 0);
	mainCamera->SetPosition(0, standingHeight, 0);
	mainCamera->SetDebugPhysicsMode(true);

	Entity* playerStart = World::GetCurrent()->FindEntity("Player Start");
	
	if(playerStart != nullptr)
	{
		entity->SetPosition(playerStart->GetPosition());
	}
	else
	{
		entity->SetPosition(0, 2, 0);
	}

	entity->SetPhysicsMode(Entity::CharacterPhysics);


	entity->SetMass(70);
}

void FirstPersonController::UpdateWorld()
{
	Print(IsGrounded());
	HandleInput();
}

void FirstPersonController::UpdatePhysics()
{

}

void FirstPersonController::Collision(Entity* entity, const Vec3& position, const Vec3& normal, float speed)
{

}

void FirstPersonController::HandleInput()
{
	if (!gameWindow || !mainCamera) { return; }
	Vec3 mouseDelta = gameWindow->GetMousePosition() - Vec3(screenCentre.x, screenCentre.y);
	gameWindow->SetMousePosition(screenCentre.x, screenCentre.y);

	Vec2 movementInput = Vec2(gameWindow->KeyDown(Key::W) - gameWindow->KeyDown(Key::S), gameWindow->KeyDown(Key::D) - gameWindow->KeyDown(Key::A)).Normalize();

	float forward = movementInput.x * walkingSpeed;
	float right = movementInput.y * walkingSpeed;
	Print(IsGrounded());
	float jump = IsGrounded() ? gameWindow->KeyHit(Key::Space) * jumpSpeed : 0;

	float turnRotation = entity->GetRotation().y + (mouseDelta.x * turnRate);
	float lookUpRotation = Math::Clamp(mainCamera->GetRotation().x + (mouseDelta.y * lookUpRate), -lookUpAngleClamp, lookUpAngleClamp);

	bool bCrouching = gameWindow->KeyDown(Key::ControlKey);
	Vec3 cameraPosition = mainCamera->GetPosition();

	bool bSprinting = gameWindow->KeyDown(Key::Shift);

	cameraPosition.y = (bCrouching && !bSprinting) ? crouchingHeight : standingHeight;

	if (bSprinting)
	{
		forward *= sprintingSpeedModifier;
		right *= sprintingSpeedModifier;
	}
	else if (bCrouching)
	{
		forward *= crouchingSpeedModifier;
		right *= crouchingSpeedModifier;
	}

	entity->SetInput(turnRotation, forward, right, jump, bCrouching);
	mainCamera->SetRotation(lookUpRotation, 0, 0);
	mainCamera->SetPosition(cameraPosition);
}

bool FirstPersonController::IsGrounded()
{
	PickInfo pickinfo;
	return World::GetCurrent()->Pick(entity->GetPosition() + Vec3(0, 0.01f, 0), entity->GetPosition() - Vec3(0, 0.01f, 0), pickinfo);
}
