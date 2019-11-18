#pragma once
#include "Leadwerks.h"

using namespace Leadwerks;

enum class MovingPlatformBehaviour
{
	LOOP,
	REVERSE,
	RESTART
};

class MovingPlatform : public Actor
{
	public:
		MovingPlatform();
		~MovingPlatform();
		virtual void Attach();
		//virtual void Detach();
		virtual void UpdateWorld();
		//virtual void UpdateMatrix();
		//virtual void UpdatePhysics();
		//virtual void Wake();
		//virtual void Sleep();
		//virtual void Collision(Entity* entity, const Vec3& position, const Vec3& normal, float speed);
		//virtual void PostRender();
		//virtual void Draw();
		//virtual void DrawEach(Camera* camera);
		//virtual void ReceiveSignal(const std::string& inputname, Entity* sender);

		void SetNextWaypoint();

	private:
		std::vector<Vec3> waypoints;
		int currentWaypointIndex;
		
		float speed;

		Joint* sliderJoint;

		MovingPlatformBehaviour behaviour;

		bool bReverseWaypoints;
};