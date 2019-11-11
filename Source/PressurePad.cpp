#include "PressurePad.h"

PressurePad::PressurePad()
{
	//collidingEntities = {};
	//collidingEntitiesThisFrame = {};
	pressed = false;
}

PressurePad::~PressurePad()
{
}

void PressurePad::Attach()
{
	Print("Pressure pad set up!");
	//entity->SetGravityMode(0);
}

void PressurePad::UpdateWorld()
{

	
}

void PressurePad::UpdatePhysics()
{

	for (auto otherEnt : collisionsThisFrame)
	{
		if ((std::find(collisionsLastFrame.begin(), collisionsLastFrame.end(), otherEnt) == collisionsLastFrame.end()))
		{
			Print("New collision with " + otherEnt->GetKeyValue("name"));
		}
		else
		{
			Print("Continuing collision with " + otherEnt->GetKeyValue("name"));
		}
	}

	for (auto otherEnt : collisionsLastFrame)
	{
		if ((std::find(collisionsThisFrame.begin(), collisionsThisFrame.end(), otherEnt) == collisionsThisFrame.end()))
		{
			Print("Ending collision with " + otherEnt->GetKeyValue("name"));
		}
	}
	collisionsLastFrame = collisionsThisFrame;
	collisionsThisFrame.clear();
	Print(static_cast<int>(collisionsLastFrame.size()));
}

void PressurePad::Collision(Entity* ent, const Vec3& position, const Vec3& normal, float speed)
{
	
	//if (ent->GetKeyValue("name") == "Player" || ent->GetKeyValue("tag") == "Interactable")
	//if (ent->GetKeyValue("tag") == "Interactable")
	if (ent->GetCollisionType() == Collision::Prop || ent->GetCollisionType() == Collision::Character)
	{
		
		if (std::find(collisionsThisFrame.begin(), collisionsThisFrame.end(), ent) == collisionsThisFrame.end())
		{
			
			collisionsThisFrame.push_back(ent);
		}
		
	}
	
}
