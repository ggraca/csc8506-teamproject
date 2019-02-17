#pragma once
#include "../Common/Vector3.h"
#include "../Common/Matrix3.h"
#include "../Common/Quaternion.h"

//#include "BulletPhysics.h"
//#include "C:\Users\Ed\Documents\GitHub\csc8506-teamproject\TeamProject\Scene.h"
//#include "C:\Users\Ed\Documents\GitHub\csc8506-teamproject\GameTechCommon\Transform.h"

using namespace NCL::Maths;

namespace NCL {
	class CollisionVolume;
	
	namespace CSC8503 {
		class Transform;

		class PhysicsObject	{
		public:
			PhysicsObject(Transform* parentTransform);
			~PhysicsObject();

			void SetMass(float mass) {
				this->mass = mass;
			}

			float GetMass() const {
				return mass;
			}

			void SetRestitution(float restitution) {
				this->restitution = restitution;
			}

			float GetRestitution() const {
				return restitution;
			}

			void SetFriction(float friction) {
				this->friction = friction;
			}

			float GetFriction() const {
				return friction;
			}

	

			void SetLinearVelocity(const Vector3& v) {
				linearVelocity = v;
			}

			void SetAngularVelocity(const Vector3& v) {
				angularVelocity = v;
			}

		protected:
			const CollisionVolume* volume;
			Transform*		transform;

			Vector3 position;
			Quaternion orientation;
			Vector3 dimensions;
			float mass;
			float restitution;
			float friction;

			//linear stuff
			Vector3 linearVelocity;
		//	Vector3 force;

			//angular stuff
			Vector3 angularVelocity;
		//	Vector3 torque
		};
	}
}

