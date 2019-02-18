#pragma once
#include "../Common/Vector3.h"
#include "../Common/Matrix3.h"
#include "../Common/Quaternion.h"
#include "../Plugins/Bullet/src/btBulletDynamicsCommon.h"

using namespace NCL::Maths;

enum ShapeType { cube, sphere, cylinder, cone, complexMesh };

namespace NCL {
	class CollisionVolume;
	
	namespace CSC8503 {
		class Transform;

		class PhysicsObject	{
		public:
			PhysicsObject();
			PhysicsObject(ShapeType type, Vector3 position, Quaternion orientation, Vector3 dimensions, float mass, float restitution, float friction);
			PhysicsObject(Transform* parentTransform, ShapeType type, float mass, float restitution, float friction);
			~PhysicsObject();

			ShapeType GetType() const {
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

			void SetTransform(Transform* transform) {
				this->transform = transform;
			}

			Transform* GetTransform() const {
				return transform;
			}



			void SetLinearVelocity(const Vector3& v) {
				linearVelocity = v;
			}

			void SetAngularVelocity(const Vector3& v) {
				angularVelocity = v;
			}

			void SetBulletPhysicsParameters();

			btCollisionShape* GetShape() {
				return shape;
			}

			btRigidBody* GetRigidbody() {
				return body;
			}

		protected:
			const CollisionVolume* volume;
			Transform*		transform;

			ShapeType type;
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

			btCollisionShape* shape;
			btRigidBody* body;
		};
	}
}

