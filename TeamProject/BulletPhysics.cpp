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

map<btRigidBody*, vector<btRigidBody*>> BulletPhysics::GenerateCollisionPairs() {
	map<btRigidBody*, vector<btRigidBody*>> collisionPairs;
	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
  
	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btRigidBody* objA = (btRigidBody*) btRigidBody::upcast(contactManifold->getBody0());
		btRigidBody* objB = (btRigidBody*) btRigidBody::upcast(contactManifold->getBody1());

		collisionPairs[objA].push_back(objB);
		collisionPairs[objB].push_back(objA);
	}

	return collisionPairs;
}

void BulletPhysics::UpdateObjectTransform(GameObject* go, btRigidBody* body) {
	Transform& transform = go->GetTransform();

	btTransform trans;
	if (body && body->getMotionState()) body->getMotionState()->getWorldTransform(trans);
	else trans = body->getWorldTransform();

	btQuaternion orientation = trans.getRotation();
	Vector3 position = Vector3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	transform.SetLocalPosition(position);
	Quaternion orient = Quaternion(orientation.x(), orientation.y(), orientation.z(), orientation.w());
	transform.SetLocalOrientation(orient);
}

void BulletPhysics::EmitOnCollisionEnterEvents(map<btRigidBody*, vector<btRigidBody*>> &collisionPairs, map<btRigidBody*, GameObject*> &collisionObjectGameObjectPair) {
	for (auto key : collisionPairs) {
		GameObject* go1 = (GameObject*)collisionObjectGameObjectPair[key.first];

		for (auto val : key.second) {
			GameObject* go2 = (GameObject*)collisionObjectGameObjectPair[val];

			if (!count(go1->collidingObjects.begin(), go1->collidingObjects.end(), go2)) {
				go1->CallOnCollisionEnterForScripts(go2);
				go1->collidingObjects.push_back(go2);
			}
			if (!count(go2->collidingObjects.begin(), go2->collidingObjects.end(), go1)) {
				go2->CallOnCollisionEnterForScripts(go1);
				go2->collidingObjects.push_back(go1);
			}
		}
	}
}

void BulletPhysics::EmitOnCollisionEndEvents(map<btRigidBody*, vector<btRigidBody*>> &collisionPairs, btRigidBody* body, GameObject*& go) {
	vector<btRigidBody*> pairs = collisionPairs[body];
	for (auto collidingGo : go->collidingObjects) {
		if (find(pairs.begin(), pairs.end(), collidingGo->GetComponent<PhysicsObject*>()->GetRigidbody()) != pairs.end()) continue;

		go->CallOnCollisionEndForScripts(collidingGo);
		go->collidingObjects.erase(remove(go->collidingObjects.begin(), go->collidingObjects.end(), collidingGo), go->collidingObjects.end());
	}
}

void BulletPhysics::UpdateBullet(float dt, int iterations) {
	dynamicsWorld->stepSimulation(dt, iterations);

	map<btRigidBody*, vector<btRigidBody*>> collisionPairs = GenerateCollisionPairs();
	map<btRigidBody*, GameObject*> collisionObjectGameObjectPair;

	for (auto& go : gameWorld.GetGameObjectList()) {
		PhysicsObject* object = go->GetComponent<PhysicsObject*>();
		if (object == nullptr) continue;

		btRigidBody* body = go->GetComponent<PhysicsObject*>()->GetRigidbody();
		UpdateObjectTransform(go, body);

		if (collisionPairs.find(body) != collisionPairs.end()) {
			collisionObjectGameObjectPair[body] = go;
		}
		
		EmitOnCollisionEndEvents(collisionPairs, body, go);
	}

	EmitOnCollisionEnterEvents(collisionPairs, collisionObjectGameObjectPair);
}
