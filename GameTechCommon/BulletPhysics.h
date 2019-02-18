#pragma once

#include "../TeamProject/GameWorld.h"
#include "../Plugins/Bullet/src/btBulletDynamicsCommon.h"
#include <map>

class BulletPhysics
{
public:
	BulletPhysics(GameWorld& g);
	~BulletPhysics();
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	void Update(float dt);
	void UpdateBullet(float dt, int iterations);
	void SetGravity(Vector3 gravity);

private:
	GameWorld& gameWorld;
	float dTOffset;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;

	std::map<const btCollisionObject*, std::vector<btManifoldPoint*>> objectsCollisions;
};
