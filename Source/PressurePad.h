#pragma once
#include "Leadwerks.h"
#include <list>
using namespace Leadwerks;


class PressurePad : public Actor
{
	public:
		PressurePad();
		~PressurePad();
		virtual void Attach();
		//virtual void Detach();
		virtual void UpdateWorld();
		//virtual void UpdateMatrix();
		virtual void UpdatePhysics();
		//virtual void Wake();
		//virtual void Sleep();
		virtual void Collision(Entity* ent, const Vec3& position, const Vec3& normal, float speed);
		//virtual void PostRender();
		//virtual void Draw();
		//virtual void DrawEach(Camera* camera);
		//virtual void ReceiveSignal(const std::string& inputname, Entity* sender);
	private:
		//std::list<Entity*> collidingEntities;
		//std::list<Entity*> collidingEntitiesThisFrame;

		std::list<Entity*> collisionsLastFrame;
		std::list<Entity*> collisionsThisFrame;
		bool pressed;

};