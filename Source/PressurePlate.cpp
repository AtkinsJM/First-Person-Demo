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

	targetEntity = World::GetCurrent()->FindEntity(entity->GetKeyValue("targetEntity"));
	if (targetEntity)
	{
		// If you want the object to collide with the scene, set collision type to scene. If you don't want it to collide with the scene, set the collision type to prop
		targetEntity->SetGravityMode(false);
		targetEntity->SetMass(1);
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
		Vec3 desiredPos = bPressed ? targetEndPos : targetStartPos;
		Vec3 movementVector = (targetEntity->GetPosition() - desiredPos);
		float distance = movementVector.Length();
		if (sliderJoint)
		{
			sliderJoint->DisableMotor();
			sliderJoint->Release();
		}
		sliderJoint = Joint::Slider(desiredPos.x, desiredPos.y, desiredPos.z, movementVector.x, movementVector.y, movementVector.z, targetEntity, nullptr);
		sliderJoint->EnableMotor();
		sliderJoint->SetMotorSpeed(targetMovementSpeed);
		sliderJoint->SetAngle(-distance);

	}
}
