#pragma once
#include "Leadwerks.h"
using namespace Leadwerks;

class FirstPersonController : public Actor
{
	public:
		FirstPersonController();
		~FirstPersonController();
		virtual void Attach();
		//virtual void Detach();
		virtual void UpdateWorld();
		//virtual void UpdateMatrix();
		virtual void UpdatePhysics();
		//virtual void Wake();
		//virtual void Sleep();
		virtual void Collision(Entity* entity, const Vec3& position, const Vec3& normal, float speed);
		//virtual void PostRender();
		//virtual void Draw();
		//virtual void DrawEach(Camera* camera);
		//virtual void ReceiveSignal(const std::string& inputname, Entity* sender);

		void HandleInput();
		bool IsGrounded();

	private:
		float walkingSpeed;
		float jumpSpeed;
		float turnRate;
		float lookUpRate;
		float lookUpAngleClamp;
		Window* gameWindow;
		Vec2 screenCentre;
		Camera* mainCamera;
		float standingHeight;
		float crouchingHeight;
		float crouchingSpeedModifier;
		float sprintingSpeedModifier;
};