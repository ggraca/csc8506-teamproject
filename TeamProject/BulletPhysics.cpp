#include "BulletPhysics.h"
#include "GameObject.h"

#include "FunctionTimer.h"

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
		if (shape) {
			collisionShapes[j] = 0;
			delete shape;
		}
	}

	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;
	collisionShapes.clear();
}

void BulletPhysics::Update(float dt) {
	FunctionTimer timer("Physics Update");

	const float iterationDt = 1.0f / 120.0f; 
	dTOffset += dt;
	int iterationCount = (int)(dTOffset / iterationDt);
	UpdateBullet(dTOffset, iterationCount);
	dTOffset -= iterationDt * iterationCount;
}

void BulletPhysics::SetGravity(Vec3 gravity)
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
	Vec3 position = Vec3((float)(trans.getOrigin().getX()), (float)(trans.getOrigin().getY()), (float)(trans.getOrigin().getZ()));
	transform.SetWorldPosition(position);
	Quaternion orient = Quaternion((float)orientation.x(), (float)orientation.y(), (float)orientation.z(), (float)orientation.w());
	transform.SetLocalOrientation(orient);
}

void BulletPhysics::UpdateObjectPhysics(GameObject* go) {

	Transform& transform = go->GetTransform();
	transform.UpdateMatrices();

	if (!go->GetComponent<PhysicsObject*>()) { return; }
	auto body = go->GetComponent<PhysicsObject*>()->GetRigidbody();
	if (!body) { return; }

	btTransform trans;
	if (body && body->getMotionState()) body->getMotionState()->getWorldTransform(trans);
	else trans = body->getWorldTransform();

	trans.setRotation(btQuaternion(transform.GetLocalOrientation().x, transform.GetLocalOrientation().y, transform.GetLocalOrientation().z, transform.GetLocalOrientation().w));
	trans.setOrigin(btVector3(transform.GetWorldPosition().x, transform.GetWorldPosition().y, transform.GetWorldPosition().z));
	
	
	body->setWorldTransform(trans);
}

void BulletPhysics::EmitOnCollisionEnterEvents(map<btRigidBody*, vector<btRigidBody*>> &collisionPairs, map<btRigidBody*, GameObject*> &collisionObjectGameObjectPair) {
	for (auto key : collisionPairs) {
		GameObject* go1 = (GameObject*)collisionObjectGameObjectPair[key.first];

		for (auto val : key.second) {
			GameObject* go2 = (GameObject*)collisionObjectGameObjectPair[val];

			if (!go1 || !go2) continue;
			if (!count(go1->collidingObjects.begin(), go1->collidingObjects.end(), go2) && 
				!count(go2->collidingObjects.begin(), go2->collidingObjects.end(), go1)) {

				go1->CallOnCollisionEnterForScripts(go2);
				go2->CallOnCollisionEnterForScripts(go1);
				go1->collidingObjects.push_back(go2);
				go2->collidingObjects.push_back(go1);
			}
		}
	}
}

void BulletPhysics::EmitOnCollisionEndEvents(map<btRigidBody*, vector<btRigidBody*>> &collisionPairs, btRigidBody* body, GameObject*& go) {
	vector<btRigidBody*> pairs = collisionPairs[body];
	for (auto collidingGo : go->collidingObjects) {
		if (!collidingGo->GetComponent<PhysicsObject*>()) continue;
		if (!collidingGo->GetComponent<PhysicsObject*>()->GetRigidbody()) continue;
		if (find(pairs.begin(), pairs.end(), collidingGo->GetComponent<PhysicsObject*>()->GetRigidbody()) != pairs.end()) continue;

		go->CallOnCollisionEndForScripts(collidingGo);
		go->collidingObjects.erase(remove(go->collidingObjects.begin(), go->collidingObjects.end(), collidingGo), go->collidingObjects.end());
	}
}

const btCollisionObject* BulletPhysics::Raycast(const Vec3& Start, const Vec3& End, Vec3& NewEnd) {
	btVector3 btStart = btVector3(Start.x, Start.y, Start.z);
	btVector3 btEnd = btVector3(End.x, End.y, End.z);
	btCollisionWorld::ClosestRayResultCallback RayCallback(btStart, btEnd);
	dynamicsWorld->rayTest(btStart, btEnd, RayCallback);
	if (RayCallback.hasHit()) {
		btVector3 btNewEnd = RayCallback.m_hitPointWorld;
		NewEnd = Vec3((const float)btNewEnd.getX(), (const float)btNewEnd.getY(), (const float)btNewEnd.getZ());
		return RayCallback.m_collisionObject;
	}
	return nullptr;
}

const btCollisionObject* BulletPhysics::RaycastPosDir(const Vec3& Pos, const Vec3& Dir, float t, Vec3& NewEnd) {
	Vec3 End = Pos + Vec3(t * Dir.x, t * Dir.y, t * Dir.z);
	return Raycast(Pos, End, NewEnd);
}

void BulletPhysics::UpdateBullet(float dt, int iterations) {

	for (auto&i : gameWorld.GetGameObjectList())
	{
		UpdateObjectPhysics(i);
	}

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
