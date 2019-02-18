#pragma once
#include "../Common/Vector3.h"
#include "../Common/Matrix3.h"
#include "../Common/Quaternion.h"

#include "../Plugins/Bullet/src/btBulletDynamicsCommon.h"
//#include "C:\Users\Ed\Documents\GitHub\csc8506-teamproject\TeamProject\Scene.h"
//#include "C:\Users\Ed\Documents\GitHub\csc8506-teamproject\GameTechCommon\Transform.h"

using namespace NCL::Maths;

enum shape { cuboid, sphere, cylinder, cone, complexMesh };

namespace NCL {
	class CollisionVolume;
	
	namespace CSC8503 {
		class Transform;

		class PhysicsObject	{
		public:
			PhysicsObject();
			PhysicsObject(shape type, Vector3 position, Quaternion orientation, Vector3 dimensions, float mass, float restitution, float friction);
			PhysicsObject(Transform* parentTransform);
			~PhysicsObject();

			shape GetType() const {
				return type;
			}

			void SetPosition(Vector3 position) {
				this->position = position;
			}

			Vector3 GetPosition() const {
				return position;
			}

			void SetOrientation(Quaternion orientation) {
				this->orientation = orientation;
			}

			Quaternion GetOrientation() const {
				return orientation;
			}

			void SetDimensions(Vector3 dimensions) {
				this->dimensions = dimensions;
			}

			Vector3 GetDimensions() const {
				return dimensions;
			}

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

//			void SetBulletPhysicsParameters(btCollisionShape* Shape, const Vector3& position, float inverseMass, float restitution, float friction, Quaternion orientation);

		protected:
			const CollisionVolume* volume;
			Transform*		transform;

			shape type;
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

