#include "PressurePlate.h"

PressurePlate::PressurePlate()
{
	springJoint = nullptr;
	bPressed = false;

	model = nullptr;

	targetEntity = nullptr;

	pressedColor = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	unpressedColor = Vec4(1.0f, 0.0f, 0.0f, 1.0f);

	targetDesiredPos = Vec3(0, 0, 0);
	targetMovementLength = 0;
	targetMovementSpeed = 1.0f;
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

	targetEntity = World::GetCurrent()->FindEntity(entity->GetKeyValue("targetEntity"));
	if (targetEntity)
	{
		targetStartPos = targetEntity->GetPosition();
		Vec3 targetMovementVector = Vec3(String::Float(entity->GetKeyValue("targetMovementX")), String::Float(entity->GetKeyValue("targetMovementY")), String::Float(entity->GetKeyValue("targetMovementZ")));
		targetEndPos = targetStartPos + targetMovementVector;
		targetDesiredPos = targetStartPos;
		targetMovementSpeed = String::Float(entity->GetKeyValue("targetLerpSpeed"));
	}
}

void PressurePlate::UpdateWorld()
{
	if ((!bPressed && entity->GetPosition().y <= pressedHeight) || bPressed && entity->GetPosition().y > pressedHeight)
	{
		TogglePressed();
	}

	HandleTargetPosition();
}

void PressurePlate::HandleTargetPosition()
{
	// Check target is not null and that target is not already at desired position
	if (targetEntity && targetMovementLength > 0.01f)
	{
		// Calculate distance already covered since start of movement
		float distCovered = ((Time::Millisecs() - targetMovementStartTime) / 1000) * targetMovementSpeed;

		// Calculate fraction of movement completed
		float movementCompleted = distCovered / targetMovementLength;

		// If movement not complete, interpolate the door's position
		if (movementCompleted < 0.99f)
		{
			Vec3 targetNewPos = targetEntity->GetPosition();
			targetNewPos.x = Math::Lerp(targetNewPos.x, targetDesiredPos.x, movementCompleted);
			targetNewPos.y = Math::Lerp(targetNewPos.y, targetDesiredPos.y, movementCompleted);
			targetNewPos.z = Math::Lerp(targetNewPos.z, targetDesiredPos.z, movementCompleted);
			targetEntity->SetPosition(targetNewPos);
		}
	}
}

void PressurePlate::UpdatePhysics()
{
	
}

void PressurePlate::TogglePressed()
{
	bPressed = !bPressed;
	if (model)
	{
		model->SetColor(bPressed ? pressedColor : unpressedColor);
	}
	if (targetEntity)
	{
		targetDesiredPos = bPressed ? targetEndPos : targetStartPos;
		targetMovementStartTime = Time::Millisecs();
		targetMovementLength = (targetEntity->GetPosition() - targetDesiredPos).Length();
	}
}
