#pragma once

#include <map>
#include <utility>

#include "../Common/Vector3.h"
#include "../Common/Vector4.h"
#include "../TeamProject/Transform.h"
#include "../TeamProject/GameWorld.h"
#include "../TeamProject/GameObject.h"
#include "../Plugins/Bullet/src/btBulletDynamicsCommon.h"

using namespace NCL;
using namespace NCL::Maths;
using namespace NCL::CSC8503;

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
	int loopNum;

	map<btRigidBody*, vector<btRigidBody*>> GenerateCollisionPairs();
	void EmitOnCollisionEndEvents(std::map<btRigidBody *, std::vector<btRigidBody *>> &collisionPairs, btRigidBody * body, NCL::CSC8503::GameObject *& go);
	void EmitOnCollisionEnterEvents(std::map<btRigidBody*, std::vector<btRigidBody*>> &collisionPairs, std::map<btRigidBody*, GameObject*> &collisionObjectGameObjectPair);
	void UpdateObjectTransform(GameObject* go, btRigidBody* body);
};