#pragma once

#include "../GameTechCommon/GameWorld.h"
#include "..\Plugins\Bullet\src\btBulletDynamicsCommon.h"

class BulletPhysics
{
public:
	BulletPhysics(GameWorld& g);
	~BulletPhysics();
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	void Update(float dt);
	void UpdateBulletPositions(float dt, int iterations);
	Vector3 gravity;

protected:
	GameWorld& gameWorld;
	float dTOffset;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
};
