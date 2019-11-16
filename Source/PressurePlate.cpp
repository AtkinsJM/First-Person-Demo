#include "PressurePlate.h"

PressurePlate::PressurePlate()
{
	springJoint = nullptr;
	bPressed = false;

	model = nullptr;

	targetEntity = nullptr;

	pressedColor = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	unpressedColor = Vec4(1.0f, 0.0f, 0.0f, 1.0f);

	targetMovementSpeed = 1.0f;

	sliderJoint = nullptr;
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

	SetUpTargetEntity();	
}

void PressurePlate::UpdateWorld()
{
	if ((!bPressed && entity->GetPosition().y <= pressedHeight) || bPressed && entity->GetPosition().y > pressedHeight)
	{
		TogglePressed();
	}
}

void PressurePlate::SetUpTargetEntity()
{
	// Find target entity to move when pressure plate is activated
	targetEntity = World::GetCurrent()->FindEntity(entity->GetKeyValue("targetEntity"));
	if (targetEntity)
	{
		// Set up correct physics
		targetEntity->SetGravityMode(false);
		targetEntity->SetMass(1);
		targetEntity->SetCollisionType(Collision::Scene);

		// Calculate start and end location and speed
		targetStartPos = targetEntity->GetPosition();
		Vec3 targetMovementVector = Vec3(String::Float(entity->GetKeyValue("targetMovementX")), String::Float(entity->GetKeyValue("targetMovementY")), String::Float(entity->GetKeyValue("targetMovementZ")));
		targetEndPos = targetStartPos + targetMovementVector;
		targetMovementSpeed = String::Float(entity->GetKeyValue("targetSpeed"));

		// Set up initial slider to hold entity in place
		sliderJoint = Joint::Slider(targetStartPos.x, targetStartPos.y, targetStartPos.z, 0, 0, 0, targetEntity, nullptr);
		sliderJoint->EnableMotor();
		sliderJoint->SetAngle(0);
	}
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
		// Calculate the desired position of the target entity
		Vec3 desiredPos = bPressed ? targetEndPos : targetStartPos;
		Vec3 movementVector = (targetEntity->GetPosition() - desiredPos);
		float distance = movementVector.Length();
		// Remove the old slider joint
		if (sliderJoint)
		{
			sliderJoint->DisableMotor();
			sliderJoint->Release();
		}
		// Set up a new slider joint
		sliderJoint = Joint::Slider(desiredPos.x, desiredPos.y, desiredPos.z, movementVector.x, movementVector.y, movementVector.z, targetEntity, nullptr);
		sliderJoint->EnableMotor();
		sliderJoint->SetMotorSpeed(targetMovementSpeed);
		sliderJoint->SetAngle(-distance);
	}
}
