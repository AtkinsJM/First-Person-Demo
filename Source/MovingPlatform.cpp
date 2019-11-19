#include "MovingPlatform.h"

MovingPlatform::MovingPlatform()
{
	currentWaypointIndex = 0;
	sliderJoint = nullptr;
	speed = 1.0f;
	behaviour = MovingPlatformBehaviour::LOOP;
	bReverseWaypoints = false;
}

MovingPlatform::~MovingPlatform()
{
}

void MovingPlatform::Attach()
{
	entity->SetGravityMode(false);
	entity->SetMass(-1);

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

	// Assign behaviour enum value
	string behaviourString = entity->GetKeyValue("endBehaviour");
	if (behaviourString == "Loop")
	{
		behaviour = MovingPlatformBehaviour::LOOP;
	}
	else if (behaviourString == "Reverse")
	{
		behaviour = MovingPlatformBehaviour::REVERSE;
	}
	else if (behaviourString == "Restart")
	{
		behaviour = MovingPlatformBehaviour::RESTART;
	}

	// Set up initial slider to hold entity in place
	sliderJoint = Joint::Slider(startPos.x, startPos.y, startPos.z, 0, 0, 0, entity, nullptr);
	sliderJoint->EnableMotor();
	sliderJoint->SetAngle(0);
}

void MovingPlatform::UpdateWorld()
{
	if (waypoints.size() == 0) { return; }

	float distance = (entity->GetPosition() - waypoints[currentWaypointIndex]).Length();
	if (distance < 0.02f && waypoints.size() > 1)
	{
		entity->SetPosition(waypoints[currentWaypointIndex]);
		// Determine next waypoint index based on behaviour
		switch (behaviour)
		{
			case MovingPlatformBehaviour::REVERSE:
				if (currentWaypointIndex == waypoints.size() - 1)
				{
					bReverseWaypoints = true;
				}
				else if (currentWaypointIndex == 0)
				{
					bReverseWaypoints = false;
				}
				currentWaypointIndex = bReverseWaypoints ? currentWaypointIndex - 1 : currentWaypointIndex + 1;
				break;
			case MovingPlatformBehaviour::RESTART:
				currentWaypointIndex++;
				if (currentWaypointIndex == waypoints.size())
				{
					currentWaypointIndex = 0;
					entity->SetPosition(waypoints[currentWaypointIndex]);
					currentWaypointIndex++;
				}
				break;
			case MovingPlatformBehaviour::LOOP:
			default:
				currentWaypointIndex = (currentWaypointIndex + 1) % waypoints.size();
				break;
		}
		SetNextWaypoint();
	}
}

void MovingPlatform::SetNextWaypoint()
{
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
