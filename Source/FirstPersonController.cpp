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

	standingHeight = 1.7f;
	crouchingHeight = 1.0f;

	crouchingSpeedModifier = 0.5f;
	sprintingSpeedModifier = 2.0f;

	characterWidth = 0.8f;
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
		
	Vec3 playerPos = playerStart ? playerStart->GetPosition() : Vec3(0, 2, 0);

	entity->SetPosition(playerPos);

	entity->SetPhysicsMode(Entity::CharacterPhysics);
	
	entity->SetMass(70);
}

void FirstPersonController::UpdateWorld()
{
	Print(IsGrounded());
	HandleInput();
}

void FirstPersonController::HandleInput()
{
	if (!gameWindow || !mainCamera) { return; }

	// Get mouse movement this frame, then recentre mouse
	Vec3 mouseDelta = gameWindow->GetMousePosition() - Vec3(screenCentre.x, screenCentre.y);
	gameWindow->SetMousePosition(screenCentre.x, screenCentre.y);

	Vec2 movementInput = Vec2(gameWindow->KeyDown(Key::W) - gameWindow->KeyDown(Key::S), gameWindow->KeyDown(Key::D) - gameWindow->KeyDown(Key::A)).Normalize();

	float forward = movementInput.x * walkingSpeed;
	float right = movementInput.y * walkingSpeed;
	float jump = IsGrounded() ? gameWindow->KeyHit(Key::Space) * jumpSpeed : 0;

	float turnRotation = entity->GetRotation().y + (mouseDelta.x * turnRate);
	float lookUpRotation = Math::Clamp(mainCamera->GetRotation().x + (mouseDelta.y * lookUpRate), -lookUpAngleClamp, lookUpAngleClamp);

	bool bCrouching = gameWindow->KeyDown(Key::ControlKey) || !CanStand();
	bool bSprinting = gameWindow->KeyDown(Key::Shift);

	// Set camera height depending on whether or not player is crouching
	Vec3 cameraPosition = mainCamera->GetPosition();
	cameraPosition.y = (bCrouching && !bSprinting) ? crouchingHeight : standingHeight;
	mainCamera->SetPosition(cameraPosition);

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
}

bool FirstPersonController::IsGrounded()
{
	PickInfo pickinfo;
	return World::GetCurrent()->Pick(entity->GetPosition() + Vec3(0, 0.01f, 0), entity->GetPosition() - Vec3(0, 0.01f, 0), pickinfo);
}

bool FirstPersonController::CanStand()
{
	PickInfo pickinfo;
	return !(World::GetCurrent()->Pick(entity->GetPosition(), entity->GetPosition() + Vec3(0, crouchingHeight + 20, 0), pickinfo) || 
		World::GetCurrent()->Pick(entity->GetPosition() + Vec3(0, 0, characterWidth/2), entity->GetPosition() + Vec3(0, crouchingHeight * 1.5f, characterWidth/2), pickinfo) ||
		World::GetCurrent()->Pick(entity->GetPosition() + Vec3(0, 0, -characterWidth / 2), entity->GetPosition() + Vec3(0, crouchingHeight * 1.5f, -characterWidth / 2), pickinfo) ||
		World::GetCurrent()->Pick(entity->GetPosition() + Vec3(characterWidth / 2, 0, 0), entity->GetPosition() + Vec3(characterWidth / 2, crouchingHeight * 1.5f, 0), pickinfo) ||
		World::GetCurrent()->Pick(entity->GetPosition() + Vec3(-characterWidth / 2, 0, 0), entity->GetPosition() + Vec3(-characterWidth / 2, crouchingHeight * 1.5f, 0), pickinfo));
}

void FirstPersonController::UpdatePhysics()
{

}

void FirstPersonController::Collision(Entity* entity, const Vec3& position, const Vec3& normal, float speed)
{

}