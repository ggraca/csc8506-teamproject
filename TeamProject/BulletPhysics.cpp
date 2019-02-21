#include "BulletPhysics.h"

using namespace NCL;
using namespace CSC8503;

BulletPhysics::BulletPhysics(GameWorld& g) : gameWorld(g)
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);;
	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));
	loopNum = 0;
}

BulletPhysics::~BulletPhysics()
{
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;
	collisionShapes.clear();
}

void BulletPhysics::Update(float dt) {
	const float iterationDt = 1.0f / 120.0f; 
	dTOffset += dt;
	int iterationCount = (int)(dTOffset / iterationDt);
	UpdateBullet(dTOffset, iterationCount);
	dTOffset -= iterationDt * iterationCount;
}

void BulletPhysics::SetGravity(Vector3 gravity)
{
	dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

GameObject* BulletPhysics::ReturnGameObject(const btCollisionObject* testColOb) {
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;
	gameWorld.GetObjectIterators(first, last);

	int j = 0;
	for (auto i = first; i != last; i++) {
		PhysicsObject* object = (*i)->GetPhysicsObject();
		if (object == nullptr) continue;
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		if (obj == testColOb) {
			return (*i);
		}
		j++;
	}
	return nullptr;
}

void BulletPhysics::UpdateBullet(float dt, int iterations) {
	playerColOb = dynamicsWorld->getCollisionObjectArray()[1];

	dynamicsWorld->stepSimulation(dt, iterations);

	//int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
	//int numCollisions = dynamicsWorld->getNumCollisionObjects();
	//cout << numManifolds << ' ' << numCollisions << endl;

	std::map<const btCollisionObject*, std::vector<btManifoldPoint*>> objectsCollisions;
	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* objA = contactManifold->getBody0();
		const btCollisionObject* objB = contactManifold->getBody1();

		if (objA != objB) {
			if (objA == playerColOb && loopNum % 2 == 0) {
				gameWorld.GetPlayerGameObject()->CallOnCollisionEnterForScripts(ReturnGameObject(objB));
				colObjects1.insert(objB);
			}

			if (objA == playerColOb && loopNum % 2 == 1) {
				gameWorld.GetPlayerGameObject()->CallOnCollisionEnterForScripts(ReturnGameObject(objB));
				colObjects2.insert(objB);
			}
		}

	/*	if (objB == playerColOb) {
			gameWorld.GetPlayerGameObject()->CallOnCollisionEnterForScripts(ReturnGameObject(objA));
		}*/

		auto& collisionsA = objectsCollisions[objA];
		auto& collisionsB = objectsCollisions[objB];
		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			collisionsA.push_back(&pt);
			collisionsB.push_back(&pt);

			const btVector3& ptA = pt.getPositionWorldOnA();
			const btVector3& ptB = pt.getPositionWorldOnB();
			const btVector3& normalOnB = pt.m_normalWorldOnB;

			/*btVector3 velA = btRigidBody::upcast(objA)->getVelocityInLocalPoint(ptA - objA->getWorldTransform().getOrigin);
			btVector3 velB = btRigidBody::upcast(objB)->getVelocityInLocalPoint(ptB - objB->getWorldTransform().getOrigin);
			Vector3 relVelOfCollision = Vector3(velA.x - velB.x, velA.y - velB.y, velA.z - velB.z);
			cout << relVelOfCollision << endl;*/
		}
	}

	if (loopNum > 0)
	{
		if (loopNum % 2 == 1)
		{
			std::set<const btCollisionObject*>::iterator it1;
			for (it1 = colObjects1.begin(); it1 != colObjects1.end(); ++it1)
			{
				bool stillColliding = false;
				std::set<const btCollisionObject*>::iterator it2;
				for (it2 = colObjects2.begin(); it2 != colObjects2.end(); ++it2)
				{				
					if (it1 == it2) {
						stillColliding = true;
						break;
					}
				}
				if (!stillColliding)
				{
					gameWorld.GetPlayerGameObject()->CallOnCollisionEndForScripts(ReturnGameObject(*it1));
				}				
			}
			colObjects1.clear();
		}
		if (loopNum % 2 == 0)
		{
			std::set<const btCollisionObject*>::iterator it2;
			for (it2 = colObjects2.begin(); it2 != colObjects2.end(); ++it2)
			{
				bool stillColliding = false;
				std::set<const btCollisionObject*>::iterator it1;
				for (it1 = colObjects1.begin(); it1 != colObjects1.end(); ++it1)
				{
					if (it1 == it2) {
						stillColliding = true;
						break;
					}
				}
				if (!stillColliding)
				{
					gameWorld.GetPlayerGameObject()->CallOnCollisionEndForScripts(ReturnGameObject(*it2));
				}
			}
			colObjects2.clear();
		}
	}

	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;
	gameWorld.GetObjectIterators(first, last);

	int j = 0;
	for (auto i = first; i != last; i++) {

		PhysicsObject* object = (*i)->GetPhysicsObject();
		if (object == nullptr) continue;

//		(*i)->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
		Transform& transform = (*i)->GetTransform();

		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);

//		body->applyForce(btVector3(400, 10, 10), btVector3(0, -10, 0));

		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		btQuaternion orientation = trans.getRotation();
		Vector3 position = Vector3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		transform.SetLocalPosition(position);
		Quaternion orient = Quaternion(orientation.x(), orientation.y(), orientation.z(), orientation.w());
		transform.SetLocalOrientation(orient);

		auto& manifoldPoints = objectsCollisions[body];
		if (!manifoldPoints.empty()) {
			//(*i)->GetRenderObject()->SetColour(Vector4(1,0,0,1));
			//(*i)->CallOnCollisionEnterForScripts((*i));
		}
		j++;
	}
	loopNum++;
}