#pragma once

#include "..\Plugins\Bullet\src\btBulletDynamicsCommon.h"

class BulletPhysics
{
public:
	BulletPhysics();
	BulletPhysics(float g);
	~BulletPhysics();
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
};
