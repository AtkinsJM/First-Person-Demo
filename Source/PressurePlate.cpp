#include "PressurePlate.h"

PressurePlate::PressurePlate()
{
	springJoint = nullptr;
	bPressed = false;

	model = nullptr;
	material = nullptr;
	pressedTex = nullptr;
	unpressedTex = nullptr;
}

PressurePlate::~PressurePlate()
{
}

void PressurePlate::Attach()
{
	// Create material
	material = Material::Create();

	// Load and apply texture
	pressedTex = Texture::Load("Materials/Developer/greengrid.tex");
	unpressedTex = Texture::Load("Materials/Developer/orangegrid.tex");
	material->SetTexture(unpressedTex);

	//Load and apply a shader
	Shader* shader = Shader::Load("Shaders/Model/diffuse.shader");
	material->SetShader(shader);
	shader->Release();

	model = entity->FindChild("Model");
	model->SetMaterial(material);

	startingPos = entity->GetPosition();
	startingRot = entity->GetRotation();
	pressedHeight = startingPos.y - 0.19f;

	// Set up spring joint
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
		material->SetTexture(bPressed ? pressedTex : unpressedTex);
		Print("Model texture changing!");
		Print(material->GetTexture());
	}
}
