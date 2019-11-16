#include "FirstPersonController.h"

#define OUT

FirstPersonController::FirstPersonController(Camera* camera)
{
	mainCamera = camera;
	window = Window::GetCurrent();
	screenCentre = Vec2(window->GetWidth() / 2, window->GetHeight() / 2);

	walkingSpeed = 4;
	jumpSpeed = 4;
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

	maxCarryMass = 40.0f;

	bCrouching = false;
	bSprinting = false;

	flashlight = nullptr;

}

FirstPersonController::~FirstPersonController()
{
}

void FirstPersonController::Attach()
{
	mainCamera->SetParent(entity);
	mainCamera->SetRotation(0, 0, 0);
	mainCamera->SetPosition(0, standingHeight, 0);
	//mainCamera->SetDebugPhysicsMode(true);

	Entity* playerStart = World::GetCurrent()->FindEntity("Player Start");

	Vec3 playerPos = playerStart ? playerStart->GetPosition() : Vec3(0, 2, 0);
	Vec3 playerRot = playerStart ? playerStart->GetRotation() : Vec3(0, 0, 0);

	entity->SetPosition(playerPos);
	entity->SetRotation(playerRot);
	entity->SetPhysicsMode(Entity::CharacterPhysics);
	entity->SetMass(80);	

	carryPivot = Pivot::Create(mainCamera);
	carryPivot->SetPosition(mainCamera->GetPosition(true) + (Forward() * reach), true);
	
	flashlight = SpotLight::Create(mainCamera);
	flashlight->SetPosition(0.4f, -0.4f, 0.4f);
	Print("Mass: " + String(entity->GetMass()));
	
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

	if (window->KeyHit(Key::F))
	{
		ToggleFlashlight();
	}
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
				if (pickInfo.entity && (pickInfo.entity->GetKeyValue("tag", "") == "Interactable") && pickInfo.entity->GetMass() <= maxCarryMass)
				{
					PickUpObject(pickInfo.entity);
				}
			}
		}
	}
	// Update carried object position and rotation via joint
	if (carriedObject && carriedObjectJoint)
	{
		carriedObjectJoint->SetTargetPosition(carryPivot->GetPosition(true), 0.40f);
		carriedObjectJoint->SetTargetRotation(entity->GetRotation(), 0.25f);
	}
}

bool FirstPersonController::IsGrounded()
{
	PickInfo pickinfo;
	return World::GetCurrent()->Pick(entity->GetPosition() + Vec3(0, 0.45f, 0), entity->GetPosition() - Vec3(0, 0.15f, 0), pickinfo, 0.40f, true);
}

bool FirstPersonController::CanStand()
{
	PickInfo pickinfo;

	return !(World::GetCurrent()->Pick(entity->GetPosition() + Vec3(0, 0.45f, 0), entity->GetPosition() + Vec3(0, crouchingHeight + 0.20f, 0), pickinfo, 0.40f, true));
}

void FirstPersonController::PickUpObject(Entity* obj)
{
	carriedObject = obj;
	carriedObject->SetGravityMode(false);
	carriedMass = carriedObject->GetMass();
	carriedObject->SetMass(1);
	carriedObject->SetCollisionType(Collision::Debris);
	carriedObjectJoint = Joint::Kinematic(0.0f, 0.0f, 0.0f, carriedObject);
}

void FirstPersonController::DropObject()
{
	carriedObject->SetGravityMode(true);
	carriedObject->SetMass(carriedMass);
	carriedMass = 0;
	carriedObject->SetCollisionType(Collision::Prop);
	carriedObjectJoint->Release();
	carriedObject = nullptr;
}

void FirstPersonController::ToggleFlashlight()
{
	if (flashlight->Hidden())
	{
		flashlight->Show();
	}
	else
	{
		flashlight->Hide();
	}
}

void FirstPersonController::UpdatePhysics()
{

}

void FirstPersonController::Collision(Entity* entity, const Vec3& position, const Vec3& normal, float speed)
{

}