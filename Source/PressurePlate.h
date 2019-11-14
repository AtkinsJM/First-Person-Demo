#pragma once

#include "Leadwerks.h"

using namespace Leadwerks;

class PressurePlate : public Actor
{
	public:
		PressurePlate();
		~PressurePlate();
		virtual void Attach();
		//virtual void Detach();
		virtual void UpdateWorld();
		//virtual void UpdateMatrix();
		virtual void UpdatePhysics();
		//virtual void Wake();
		//virtual void Sleep();
		//virtual void Collision(Entity* entity, const Vec3& position, const Vec3& normal, float speed);
		//virtual void PostRender();
		//virtual void Draw();
		//virtual void DrawEach(Camera* camera);
		//virtual void ReceiveSignal(const std::string& inputname, Entity* sender);

		void TogglePressed();
	private:
		Vec3 startingPos;
		Vec3 startingRot;
		Joint* springJoint;
		float pressedHeight;
		bool bPressed;

		Entity* model;
		Vec4 pressedColor;
		Vec4 unpressedColor;
		Material* material;
};