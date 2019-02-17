#pragma once

#include "../GameTechCommon/GameWorld.h"
#include "../Plugins/Bullet/src/btBulletDynamicsCommon.h"

class BulletPhysics
{
public:
	BulletPhysics(/*GameWorld& g*/);
	~BulletPhysics();
	btDiscreteDynamicsWorld* dynamicsWorld; //TODO Make this private???
	btAlignedObjectArray<btCollisionShape*> collisionShapes; //TODO Make this private???
	void Update(float dt);
	void UpdateBullet(float dt, int iterations);
	void SetGravity(Vector3 gravity);

private:
	/*GameWorld& gameWorld;*/
	float dTOffset;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;

	std::map<const btCollisionObject*, std::vector<btManifoldPoint*>> objectsCollisions;
};
