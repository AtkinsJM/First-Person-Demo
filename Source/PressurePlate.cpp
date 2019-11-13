#include "PressurePlate.h"

PressurePlate::PressurePlate()
{
	springJoint = nullptr;
	bPressed = false;
	
	unpressedTexture = nullptr;
	pressedTexture = nullptr;
	material = nullptr;
	pressedMat = nullptr;
	unpressedMat = nullptr;
	model = nullptr;
}

PressurePlate::~PressurePlate()
{
}

void PressurePlate::Attach()
{
	pressedMat = Material::Load("Materials/Developer/greengrid.mat");
	unpressedMat = Material::Load("Materials/Developer/orangegrid.mat");
	model = entity->GetChild(0);

	startingPos = entity->GetPosition();
	startingRot = entity->GetRotation();
	pressedHeight = startingPos.y - 0.19f;
	springJoint = Joint::Kinematic(0, 0, 0, entity);
	int requiredMass = String::Int(entity->GetKeyValue("requiredMass"));
	float friction = 100.0f + 5.0f * requiredMass;
	springJoint->SetFriction(5000.0f, friction);

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
	springJoint->SetTargetPosition(startingPos, 3.0f);
}

void PressurePlate::TogglePressed()
{
	bPressed = !bPressed;
	if (model)
	{
		model->SetMaterial(bPressed ? pressedMat : unpressedMat);
	}
}
