#include "PhysicsObject.h"
#include "../TeamProject/Transform.h"

using namespace NCL;
using namespace CSC8503;

PhysicsObject::PhysicsObject() {
	dimensions = Vector3(1, 1, 1);
	orientation = Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0);
	position = Vector3(0, 0, 0);
	mass = 1.0f;
	restitution = 0.8f;
	friction = 0.8f;


	//btCollisionShape* Shape = new btBoxShape(btVector3(btScalar(dimensions.x), btScalar(dimensions.y), btScalar(dimensions.z)));
	//SetBulletPhysicsParameters(Shape, position, mass, restitution, friction, orientation);
}

PhysicsObject::PhysicsObject(shape type, Vector3 position, Quaternion orientation, Vector3 dimensions, float mass, float restitution, float friction) {
	this->type = type;
	this->dimensions = dimensions;
	this->orientation = orientation;
	this->position = position;
	this->mass = mass;
	this->restitution = restitution;
	this->friction = friction;


	//btCollisionShape* Shape = new btBoxShape(btVector3(btScalar(dimensions.x), btScalar(dimensions.y), btScalar(dimensions.z)));
	//SetBulletPhysicsParameters(Shape, position, mass, restitution, friction, orientation);
}




PhysicsObject::PhysicsObject(Transform* parentTransform)	{
	transform	= parentTransform;
	dimensions = Vector3(1, 1, 1);
	orientation = Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0);
	position = Vector3(0, 0, 0);
	mass = 1.0f;
	restitution = 0.8f;
	friction = 0.8f;


	//btCollisionShape* Shape = new btBoxShape(btVector3(btScalar(dimensions.x), btScalar(dimensions.y), btScalar(dimensions.z)));
	//SetBulletPhysicsParameters(Shape, position, mass, restitution, friction, orientation);
}

PhysicsObject::~PhysicsObject()	{

}

//void PhysicsObject::SetBulletPhysicsParameters(btCollisionShape* Shape, const Vector3& position, float inverseMass, float restitution, float friction, Quaternion orientation)
//{
//	physics->collisionShapes.push_back(Shape);
//	btTransform Transform;
//	btQuaternion orient = btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w);
//	Transform.setIdentity();
//	Transform.setOrigin(btVector3(position.x, position.y, position.z));
//	Transform.setRotation(orient);
//	btScalar mass(inverseMass);
//	bool isDynamic = (mass != 0.0f);
//	btVector3 localInertia(0, 0, 0);
//	if (isDynamic)
//		Shape->calculateLocalInertia(mass, localInertia);
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(Transform);
//	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, Shape, localInertia);
//	btRigidBody* body = new btRigidBody(rbInfo);
//	physics->dynamicsWorld->addRigidBody(body);
//	body->setFriction(0.4); //TODO Now physical properties can be set per object by passing parameters to this function from AddSphereToWorld/AddCubeToWorld, with default values when omitted. Will sort this v. soon!
//	body->setRestitution(0.9);
//	body->setRollingFriction(0.9);
//	body->setSpinningFriction(0.3);
//}