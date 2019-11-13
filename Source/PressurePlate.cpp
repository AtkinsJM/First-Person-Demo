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
}

PressurePlate::~PressurePlate()
{
}

void PressurePlate::Attach()
{
	pressedMat = Material::Load("Materials/Developer/greengrid.mat");
	unpressedMat = Material::Load("Materials/Developer/orangegrid.mat");
	
	startingPos = entity->GetPosition();
	startingRot = entity->GetRotation();
	pressedHeight = startingPos.y - 0.18f;
	springJoint = Joint::Kinematic(0, 0, 0, entity);
	springJoint->SetFriction(5000.0f, 3500.0f);

}

void PressurePlate::UpdateWorld()
{
	if ((!bPressed && entity->GetPosition().y <= pressedHeight) || bPressed && entity->GetPosition().y > pressedHeight)
	{
		TogglePressed();
	}
}

void PressurePlate::TogglePressed()
{
	bPressed = !bPressed;
	entity->GetChild(0)->SetMaterial(bPressed ? pressedMat : unpressedMat);
}
