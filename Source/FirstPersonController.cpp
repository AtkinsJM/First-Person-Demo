#include "FirstPersonController.h"

#define OUT

FirstPersonController::FirstPersonController(Camera* camera)
{
	mainCamera = camera;
	window = Window::GetCurrent();
	screenCentre = Vec2(window->GetWidth() / 2, window->GetHeight() / 2);

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

	reach = 2.0f;
	carriedObjectJoint = nullptr;
	carriedObject = nullptr;
	carryPivot = nullptr;

	maxCarryMass = 30.0f;

	bCrouching = false;
	bSprinting = false;

}

FirstPersonController::~FirstPersonController()
{
}

void FirstPersonController::Attach()
{
	mainCamera->SetParent(entity);
	mainCamera->SetRotation(0, 0, 0);
	mainCamera->SetPosition(0, standingHeight, 0);
	mainCamera->SetDebugPhysicsMode(true);

	Entity* playerStart = World::GetCurrent()->FindEntity("Player Start");

	Vec3 playerPos = playerStart ? playerStart->GetPosition() : Vec3(0, 2, 0);

	entity->SetPosition(playerPos);
	entity->SetPhysicsMode(Entity::CharacterPhysics);
	entity->SetMass(70);	

	carryPivot = Pivot::Create(mainCamera);
	carryPivot->SetPosition(mainCamera->GetPosition(true) + (Forward() * reach), true);
}

void FirstPersonController::UpdateWorld()
{
	HandleInput();

	HandleCarrying();	
}

void FirstPersonController::HandleInput()
{
	if (!window || !mainCamera) { return; }

	// Get mouse movement this frame, then recentre mouse
	Vec3 mouseDelta = window->GetMousePosition() - Vec3(screenCentre.x, screenCentre.y);
	window->SetMousePosition(screenCentre.x, screenCentre.y);

	Vec2 movementInput = Vec2(window->KeyDown(Key::W) - window->KeyDown(Key::S), window->KeyDown(Key::D) - window->KeyDown(Key::A)).Normalize();

	float forward = movementInput.x * walkingSpeed;
	float right = movementInput.y * walkingSpeed;
	float jump = IsGrounded() ? window->KeyHit(Key::Space) * jumpSpeed : 0;

	float turnRotation = entity->GetRotation().y + (mouseDelta.x * turnRate);
	float lookUpRotation = Math::Clamp(mainCamera->GetRotation().x + (mouseDelta.y * lookUpRate), -lookUpAngleClamp, lookUpAngleClamp);

	bCrouching = window->KeyDown(Key::ControlKey) || (bCrouching && !CanStand());
	bSprinting = window->KeyDown(Key::Shift);

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

void FirstPersonController::HandleCarrying()
{
	if (window->KeyHit(Key::E))
	{
		// If currently carrying an object, drop it
		if (carriedObject)
		{
			DropObject();
		}
		else
		{
			// If there's an object in reach that is interactable, pick it up
			PickInfo pickInfo;
			if (World::GetCurrent()->Pick(mainCamera->GetPosition(true), mainCamera->GetPosition(true) + (Forward() * reach), OUT pickInfo))
			{
				if (pickInfo.entity && (pickInfo.entity->GetKeyValue("tag", "") == "Interactable"))
				{
					PickUpObject(pickInfo.entity);
				}
			}
		}
	}
	// Update carried object position and rotation via joint
	if (carriedObject && carriedObjectJoint)
	{
		carriedObjectJoint->SetTargetPosition(carryPivot->GetPosition(true), 0.45f);
		carriedObjectJoint->SetTargetRotation(entity->GetRotation(), 0.25f);
	}
}

bool FirstPersonController::IsGrounded()
{
	PickInfo pickinfo;
	return World::GetCurrent()->Pick(entity->GetPosition() + Vec3(0, 0.15f, 0), entity->GetPosition() - Vec3(0, 0.15f, 0), pickinfo);
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

void FirstPersonController::PickUpObject(Entity* obj)
{
	carriedObject = obj;
	carriedObject->SetGravityMode(false);
	carriedObjectJoint = Joint::Kinematic(0.0f, 0.0f, 0.0f, carriedObject);
	Print(carriedObject->GetCollisionType());
}

void FirstPersonController::DropObject()
{
	carriedObject->SetGravityMode(true);
	carriedObjectJoint->Release();
	carriedObject = nullptr;
}

void FirstPersonController::UpdatePhysics()
{

}

void FirstPersonController::Collision(Entity* entity, const Vec3& position, const Vec3& normal, float speed)
{

}