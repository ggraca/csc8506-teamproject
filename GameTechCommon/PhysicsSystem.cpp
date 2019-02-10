#include "PhysicsSystem.h"
#include "PhysicsObject.h"
#include "GameObject.h"
#include "CollisionDetection.h"
#include "../Common/Quaternion.h"

#include "Constraint.h"

#include "Debug.h"

#include <functional>
using namespace NCL;
using namespace CSC8503;

/*

These two variables help define the relationship between positions
and the forces that are added to objects to change those positions

*/

const float PhysicsSystem::UNIT_MULTIPLIER = 1.0f;
const float PhysicsSystem::UNIT_RECIPROCAL = 1.0f / UNIT_MULTIPLIER;

PhysicsSystem::PhysicsSystem(GameWorld& g) : gameWorld(g)	{

//	bulletPhysics = new BulletPhysics();

	applyGravity	= true;
	useBroadPhase	= true;
	dTOffset		= 0.0f;
	globalDamping	= 0.95f;
	SetGravity(Vector3(0.0f, -9.8f, 0.0f));
}

PhysicsSystem::~PhysicsSystem()	{
//	delete bulletPhysics;

}

void PhysicsSystem::SetGravity(const Vector3& g) {
	gravity = g * UNIT_MULTIPLIER;
}

void PhysicsSystem::Clear() {
	allCollisions.clear();
}

void PhysicsSystem::Update(float dt) {
	const float iterationDt = 1.0f / 120.0f; //Ideally we'll have 120 physics updates a second
	dTOffset += dt; //We accumulate time delta here - there might be remainders from previous frame!
	int iterationCount = (int)(dTOffset / iterationDt); //And split it up here

	UpdateBulletPositions(dTOffset, iterationCount);

	dTOffset -= iterationDt * iterationCount;

	ClearForces();	//Once we've finished with the forces, reset them to zero

	
}

void PhysicsSystem::ClearForces() {
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;
	gameWorld.GetObjectIterators(first, last);

	for (auto i = first; i != last; ++i) {
		//Clear our object's forces for the next frame
		(*i)->GetPhysicsObject()->ClearForces();
	}
}

void PhysicsSystem::UpdateConstraints(float dt) {
	std::vector<Constraint*>::const_iterator first;
	std::vector<Constraint*>::const_iterator last;
	gameWorld.GetConstraintIterators(first, last);

	for (auto i = first; i != last; ++i) {
		(*i)->UpdateConstraint(dt);
	}
}

void PhysicsSystem::UpdateBulletPositions(float dt, int iterations) {

	bulletPhysics->dynamicsWorld->stepSimulation(1.f / 60.f, 1);

	//std::vector<GameObject*>::const_iterator first;
	//std::vector<GameObject*>::const_iterator last;
	//gameWorld.GetObjectIterators(first, last);
	//int j = 0;
	//for (auto i = first; i != last; i++) {
	//	
	//	PhysicsObject* object = (*i)->GetPhysicsObject();
	//	if (object == nullptr) continue;

	//	Transform& transform = (*i)->GetTransform();

	//	btCollisionObject* obj = bulletPhysics->dynamicsWorld->getCollisionObjectArray()[j]; //TODO This will only work if all gameWorld objects are physics objects!
	//	btRigidBody* body = btRigidBody::upcast(obj);
	//	btTransform trans;
	//	if (body && body->getMotionState())
	//	{
	//		body->getMotionState()->getWorldTransform(trans);
	//	}
	//	else
	//	{
	//		trans = obj->getWorldTransform();
	//	}
	////	Vector3 position = Vector3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	////	cout << position.x << ' ' << position.y << ' ' << position.z;
	//////	transform.SetLocalPosition(position);
	//	j++;
	////	/*transform.SetWorldPosition(position);*/
	//}
//	for (int j = bulletPhysics->dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	//for (int j = 1; j < bulletPhysics->dynamicsWorld->getNumCollisionObjects(); j++)
	//{
	//	btCollisionObject* obj = bulletPhysics->dynamicsWorld->getCollisionObjectArray()[j];
	//	btRigidBody* body = btRigidBody::upcast(obj);
	//	btTransform trans;
	//	if (body && body->getMotionState())
	//	{
	//		body->getMotionState()->getWorldTransform(trans);
	//	}
	//	else
	//	{
	//		trans = obj->getWorldTransform();
	//	}
	//	//printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	//	/*double x = trans.getOrigin().getX();
	//	double y = trans.getOrigin().getY();
	//	double z = trans.getOrigin().getZ();*/
	//	/*Vector3 position = Vector3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	//	cout << position.x << ' ' << position.y << ' ' << position.z;*/
	//	

	//}
	
		btCollisionObject* obj = bulletPhysics->dynamicsWorld->getCollisionObjectArray()[0];
		btTransform trans = obj->getWorldTransform();

		/*btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}*/
	

	
		obj = bulletPhysics->dynamicsWorld->getCollisionObjectArray()[1];
		trans = obj->getWorldTransform();



		/*btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}*/
	

	
		obj = bulletPhysics->dynamicsWorld->getCollisionObjectArray()[2];
		trans = obj->getWorldTransform();


		/*btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}*/
	

//	bulletPhysics->dynamicsWorld->stepSimulation(dt, iterations);
}