#pragma once
#include "../Common/Vector3.h"
#include "../Common/Matrix3.h"
#include "../Common/Quaternion.h"
#include "../Plugins/Bullet/src/btBulletDynamicsCommon.h"
#include "Component.h"

using namespace NCL::Maths;

enum ShapeType { cube, sphere, cylinder, cone, complexMesh };

namespace NCL {
	class CollisionVolume;
	
	namespace CSC8503 {
		class Transform;
		
		class PhysicsObject : virtual public Component {
		public:
			PhysicsObject(Transform* parentTransform, ShapeType type, float mass, float restitution = 0.9f, float friction = 0.5f);
			~PhysicsObject();

			ShapeType GetType() const {
				return type;
			}

			void SetPosition(Vector3 position) {
				body->activate();
				body->getWorldTransform().setOrigin(btVector3(position.x, position.y, position.z));
				this->position = position;
			}

			Vector3 GetPosition() const {
				return position;
			}

			void SetOrientation(Quaternion orientation) {
				btQuaternion orient = btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w);
				body->activate();
				body->getWorldTransform().setRotation(orient);
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
				body->activate();
				body->setLinearVelocity(btVector3(v.x, v.y, v.z));
				linearVelocity = v;
			}

			void SetAngularVelocity(const Vector3& v) {
				body->activate();
				body->setAngularVelocity(btVector3(v.x, v.y, v.z));
				angularVelocity = v;
			}

			void ApplyForce(const Vector3& f, const Vector3& p) {
				body->activate();
				body->applyForce(btVector3(f.x, f.y, f.z), btVector3(p.x, p.y, p.z));
				force = f;
			}

			void ApplyTorque(const Vector3& t) {
				body->activate();
				body->applyTorque(btVector3(t.x, t.y, t.z));
				torque = t;
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
			
			Vector3 linearVelocity; //TODO Remove if not needed
			Vector3 force;          //TODO Remove if not needed

			Vector3 angularVelocity;//TODO Remove if not needed
			Vector3 torque;         //TODO Remove if not needed

			btCollisionShape* shape;
			btRigidBody* body;
		};
	}
}

