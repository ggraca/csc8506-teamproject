#pragma once

#include "../GameTechCommon/GameWorld.h"
#include <set>
#include "..\Plugins\Bullet\src\btBulletDynamicsCommon.h"

#include "BulletPhysics.h"
#include "..\TeamProject\Scene.h"

namespace NCL {
	namespace CSC8503 {
		class PhysicsSystem	{
		public:
			
			PhysicsSystem(GameWorld& g);
			~PhysicsSystem();

			void Clear();

			void Update(float dt);

			void UseGravity(bool state) {
				applyGravity = state;
			}

			void SetGlobalDamping(float d) {
				globalDamping = d;
			}

			void SetGravity(const Vector3& g);

			static const float UNIT_MULTIPLIER;
			static const float UNIT_RECIPROCAL;

			BulletPhysics*      bulletPhysics;

			void UpdateBulletPositions(float dt, int iterations);

		protected:

			

			

			void BasicCollisionDetection();
			void BroadPhase();
			void NarrowPhase();

			void ClearForces();

			void IntegrateAccel(float dt);
			void IntegrateVelocity(float dt);

			void UpdateConstraints(float dt);

			void UpdateCollisionList();

			void ImpulseResolveCollision(GameObject& a , GameObject&b, CollisionDetection::ContactPoint& p) const;

			GameWorld& gameWorld;

			bool	applyGravity;
			Vector3 gravity;
			float	dTOffset;
			float	globalDamping;

			std::set<CollisionDetection::CollisionInfo> allCollisions;
			std::set<CollisionDetection::CollisionInfo> broadphaseCollisions;
			bool useBroadPhase		= true;
			int numCollisionFrames	= 5;
		};
	}
}

