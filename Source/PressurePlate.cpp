#include "PressurePlate.h"

PressurePlate::PressurePlate()
{
	springJoint = nullptr;
	bPressed = false;

	model = nullptr;

	triggerDoor = nullptr;

	pressedColor = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	unpressedColor = Vec4(1.0f, 0.0f, 0.0f, 1.0f);

	doorTargetPos = Vec3(0, 0, 0);
}

PressurePlate::~PressurePlate()
{
}

void PressurePlate::Attach()
{
	// Assign model, then set material and color
	model = entity->FindChild("Model");
	if (model)
	{
		model->SetColor(unpressedColor);
	}
	
	startingPos = entity->GetPosition();
	startingRot = entity->GetRotation();
	pressedHeight = startingPos.y - 0.18f;

	// Set up spring joint
	springJoint = Joint::Kinematic(0, 0, 0, entity);
	int requiredMass = String::Int(entity->GetKeyValue("requiredMass"));
	// TODO: fine-tune equation
	float friction = 85.0f + 5.0f * requiredMass;
	springJoint->SetFriction(5000.0f, friction);

	triggerDoor = World::GetCurrent()->FindEntity(entity->GetKeyValue("triggerDoor"));
	if (triggerDoor)
	{
		doorClosedPos = triggerDoor->GetPosition();
		Vec3 doorMovement = Vec3(String::Float(entity->GetKeyValue("doorOpenMovementX")), String::Float(entity->GetKeyValue("doorOpenMovementY")), String::Float(entity->GetKeyValue("doorOpenMovementZ")));
		doorOpenPos = doorClosedPos + doorMovement;
		doorTargetPos = doorClosedPos;
	}
}

void PressurePlate::UpdateWorld()
{
	if ((!bPressed && entity->GetPosition().y <= pressedHeight) || bPressed && entity->GetPosition().y > pressedHeight)
	{
		TogglePressed();
	}
}

void PressurePlate::UpdatePhysics()
{
	
	if (triggerDoor)
	{
		Vec3 doorDesiredPos = triggerDoor->GetPosition();

		// TODO: specify lerp time properly (0-1)
		// Move to UpdateWorld?
		doorDesiredPos.x = Math::Lerp(doorDesiredPos.x, doorTargetPos.x, 0.1f);
		doorDesiredPos.y = Math::Lerp(doorDesiredPos.y, doorTargetPos.y, 0.1f);
		doorDesiredPos.z = Math::Lerp(doorDesiredPos.z, doorTargetPos.z, 0.1f);
		triggerDoor->SetPosition(doorDesiredPos);

	}
	//springJoint->SetTargetPosition(startingPos, 3.0f);
}

void PressurePlate::TogglePressed()
{
	bPressed = !bPressed;
	if (model)
	{
		model->SetColor(bPressed ? pressedColor : unpressedColor);
	}
	if (triggerDoor)
	{
		//triggerDoor->Move(bPressed ? Vec3(0, 2, 0) : Vec3(0, -2, 0));
		doorTargetPos = bPressed ? doorOpenPos : doorClosedPos;
	}
	
}
