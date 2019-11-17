#include "MovingPlatform.h"

MovingPlatform::MovingPlatform()
{
	currentWaypointIndex = 0;
	sliderJoint = nullptr;
	speed = 1.0f;
}

MovingPlatform::~MovingPlatform()
{
}

void MovingPlatform::Attach()
{
	entity->SetGravityMode(false);
	entity->SetMass(1);

	int numWaypoints = String::Int(entity->GetKeyValue("numWaypoints"));
	Vec3 startPos = entity->GetPosition(true);
	waypoints.push_back(startPos);
	
	if (numWaypoints > 0)
	{
		for (int i = 1; i <= numWaypoints; i++)
		{
			string keyName = "waypoint" + String(i);			
			if (entity->GetKeyValue(keyName) != "")
			{
				Entity* waypoint = World::GetCurrent()->FindEntity(entity->GetKeyValue(keyName));
				if (waypoint)
				{
					waypoints.push_back(waypoint->GetPosition(true));
				}
			}
			
		}
	}
	speed = String::Float(entity->GetKeyValue("speed"));

	// Set up initial slider to hold entity in place
	sliderJoint = Joint::Slider(startPos.x, startPos.y, startPos.z, 0, 0, 0, entity, nullptr);
	sliderJoint->EnableMotor();
	sliderJoint->SetAngle(0);
}

void MovingPlatform::UpdateWorld()
{
	if (waypoints.size() == 0) { return; }

	float distance = (entity->GetPosition() - waypoints[currentWaypointIndex]).Length();
	if (distance < 0.01f && waypoints.size() > 1)
	{
		SetNextWaypoint();
	}
}

void MovingPlatform::SetNextWaypoint()
{
	currentWaypointIndex = (currentWaypointIndex+1) % waypoints.size();

	Vec3 newWaypoint = waypoints[currentWaypointIndex];
	Vec3 movementVector = (entity->GetPosition() - newWaypoint);
	float distance = movementVector.Length();

	// Remove the old slider joint
	if (sliderJoint)
	{
		sliderJoint->DisableMotor();
		sliderJoint->Release();
	}
	// Set up a new slider joint
	sliderJoint = Joint::Slider(newWaypoint.x, newWaypoint.y, newWaypoint.z, movementVector.x, movementVector.y, movementVector.z, entity, nullptr);
	sliderJoint->EnableMotor();
	sliderJoint->SetMotorSpeed(speed);
	sliderJoint->SetAngle(-distance);
	
}
