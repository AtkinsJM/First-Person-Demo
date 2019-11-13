#include "PressurePlate.h"

PressurePlate::PressurePlate()
{
	springJoint = nullptr;
	bPressed = false;
	
	unpressedMat = nullptr;
	pressedMat = nullptr;

}

PressurePlate::~PressurePlate()
{
}

void PressurePlate::Attach()
{
	pressedMat = Material::Load("Materials/Developer/greengrid.mat");
	unpressedMat = Material::Load("Materials/Developer/bluegrid.mat");

	startingPos = entity->GetPosition();
	startingRot = entity->GetRotation();
	pressedHeight = startingPos.y - 0.30f;
	springJoint = Joint::Kinematic(0, 0, 0, entity);
	springJoint->SetFriction(5000.0f, 3500.0f);
	springJoint->SetTargetPosition(entity->GetPosition());

}

void PressurePlate::UpdateWorld()
{
	if (!bPressed && entity->GetPosition().y <= pressedHeight)
	{
		TogglePressed();
	}
	if (bPressed && entity->GetPosition().y > pressedHeight)
	{
		TogglePressed();
		
	}

}

void PressurePlate::TogglePressed()
{
	bPressed = !bPressed;
	// TODO: work out why material isn't being updated
	entity->SetMaterial(bPressed ? pressedMat : unpressedMat);
}
