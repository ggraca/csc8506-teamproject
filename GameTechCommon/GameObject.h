#pragma once
#include "Transform.h"
#include "CollisionVolume.h"

#include "PhysicsObject.h"
#include "RenderObject.h"
#include "NetworkObject.h"
#include "Layer.h"

#include <vector>

using std::vector;

namespace NCL {
	namespace CSC8503 {
		class NetworkObject;

		class GameObject	{
		public:
			GameObject(string name = "");
			~GameObject();

			void SetBoundingVolume(CollisionVolume* vol) {
				boundingVolume = vol;
			}

			const CollisionVolume* GetBoundingVolume() const {
				return boundingVolume;
			}

			bool IsActive() const {
				return isActive;
			}

			const Transform& GetConstTransform() const {
				return transform;
			}

			Transform& GetTransform() {
				return transform;
			}

			RenderObject* GetRenderObject() const {
				return renderObject;
			}

			PhysicsObject* GetPhysicsObject() const {
				return physicsObject;
			}

			NetworkObject* GetNetworkObject() const {
				return networkObject;
			}

			void SetRenderObject(RenderObject* newObject) {
				renderObject = newObject;
			}

			void SetPhysicsObject(PhysicsObject* newObject) {
				physicsObject = newObject;
			}

			const string& GetName() const {
				return name;
			}

			void SetName(string s) {
				name = s;
			}

			void SetLayer(Layer::ObjectLayer l)
			{
				this->layer = l;
			}

			Layer::ObjectLayer GetLayer() const
			{
				return this->layer;
			}


			virtual void OnCollisionBegin(GameObject* otherObject) {
				
			}

			virtual void OnCollisionEnd(GameObject* otherObject) {
				
			}

			bool InsideAABB(const Vector3& pos, const Vector3& halfSize);

			
			
		protected:
			Transform			transform;

			CollisionVolume*	boundingVolume;
			PhysicsObject*		physicsObject;
			RenderObject*		renderObject;
			NetworkObject*		networkObject;
			Layer::ObjectLayer  layer;

			bool	isActive;
			string	name;
			
		};
	}
}
