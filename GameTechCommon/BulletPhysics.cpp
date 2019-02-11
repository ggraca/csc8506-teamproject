#include "BulletPhysics.h"

BulletPhysics::BulletPhysics(GameWorld& g) : gameWorld(g)
{
	gravity = Vector3(0, -9.81, 0);
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);;
	dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	collisionShapes.clear(); //TODO Delete this, as not strictly necessary?
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
	dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));  //TODO Eventually remove this?
	const float iterationDt = 1.0f / 120.0f; 
	dTOffset += dt;
	int iterationCount = (int)(dTOffset / iterationDt);
	UpdateBulletPositions(dTOffset, iterationCount);
	dTOffset -= iterationDt * iterationCount;
}

void BulletPhysics::UpdateBulletPositions(float dt, int iterations) {
	dynamicsWorld->stepSimulation(dt, iterations);

	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;
	gameWorld.GetObjectIterators(first, last);

	int j = 0;
	for (auto i = first; i != last; i++) {

		/*PhysicsObject* object = (*i)->GetPhysicsObject();
		if (object == nullptr) continue;*/

		Transform& transform = (*i)->GetTransform();

		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j]; //TODO This will only work if all gameWorld objects are physics objects!
		obj->setFriction(0.4);
		obj->setRestitution(0.9);
		obj->setRollingFriction(0.9);
		obj->setSpinningFriction(0.3);
		btRigidBody* body = btRigidBody::upcast(obj);
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
		j++;
	}
}