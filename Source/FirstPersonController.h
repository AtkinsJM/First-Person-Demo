#pragma once
#include "Leadwerks.h"
using namespace Leadwerks;

class FirstPersonController : public Actor
{
	public:
		FirstPersonController(Camera* camera);
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
		void HandleCarrying();
		bool IsGrounded();
		bool CanStand();

		void PickUpObject(Entity* obj);
		void DropObject();

		void ToggleFlashlight();

		FORCEINLINE Vec3 Forward() { return TFormVector(0, 0, 1, mainCamera, NULL); }

	private:
		float walkingSpeed;
		float jumpSpeed;
		float turnRate;
		float lookUpRate;
		float lookUpAngleClamp;
		Window* window;
		Vec2 screenCentre;
		Camera* mainCamera;
		float standingHeight;
		float crouchingHeight;
		float crouchingSpeedModifier;
		float sprintingSpeedModifier;
		float characterWidth;
		float reach;
		float maxCarryMass;

		Joint* carriedObjectJoint;
		Entity* carriedObject;
		Entity* carryPivot;

		bool bCrouching;
		bool bSprinting;

		float carriedMass;

		SpotLight* flashlight;
};