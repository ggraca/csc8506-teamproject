#pragma once

#include <map>
#include <utility>

#include "../Common/Vector3.h"
#include "../Common/Vector4.h"
#include "Transform.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "../Plugins/Bullet/src/btBulletDynamicsCommon.h"

using namespace NCL;
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
	const btCollisionObject* Raycast(const Vector3& Start, const Vector3& End, Vector3& NewEnd);
	const btCollisionObject* RaycastPosDir(const Vector3& Pos, const Vector3& Dir, float t, Vector3& NewEnd);

private:
	GameWorld& gameWorld;
	float dTOffset;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;

	map<btRigidBody*, vector<btRigidBody*>> GenerateCollisionPairs();
	void EmitOnCollisionEndEvents(map<btRigidBody *, vector<btRigidBody *>> &collisionPairs, btRigidBody * body, GameObject *& go);
	void EmitOnCollisionEnterEvents(map<btRigidBody*, vector<btRigidBody*>> &collisionPairs, map<btRigidBody*, GameObject*> &collisionObjectGameObjectPair);
	void UpdateObjectTransform(GameObject* go, btRigidBody* body);
	void UpdateObjectPhysics(GameObject * go);
};