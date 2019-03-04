#include "PhysicsObject.h"
#include "../TeamProject/Transform.h"
#include "../TeamProject/Scene.h"

using namespace NCL;
using namespace CSC8503;

PhysicsObject::PhysicsObject() {
	dimensions = Vector3(1, 1, 1); //TODO Make transform from these parameters, or delete constructor
	orientation = Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0);
	position = Vector3(0, 0, 0);
	mass = 1.0f;
	restitution = 0.3f;
	friction = 0.3f;
}

PhysicsObject::PhysicsObject(ShapeType type, Vector3 position, Quaternion orientation, Vector3 dimensions, float mass, float restitution, float friction) {
	this->type = type;
	this->dimensions = dimensions; //TODO Make transform from these parameters, or delete constructor
	this->orientation = orientation;
	this->position = position;
	this->mass = mass;
	this->restitution = restitution;
	this->friction = friction;
}

PhysicsObject::PhysicsObject(Transform* parentTransform, ShapeType type, float mass, float restitution, float friction)	{
	transform = parentTransform;
	this->type = type;
	this->mass = mass;
	this->restitution = restitution;
	this->friction = friction;

	Vector3 dimensions = transform->GetLocalScale();
	
	if (type == cube) {
		shape = new btBoxShape(btVector3(btScalar(dimensions.x), btScalar(dimensions.y), btScalar(dimensions.z)));
	}
	if (type == sphere) {
		shape = new btSphereShape(btScalar(dimensions.x));
	}
	if (type == cylinder) {
		shape = new btCylinderShape(btVector3(btScalar(dimensions.x), btScalar(dimensions.y), btScalar(dimensions.z)));
	}
	if (type == cone) {
		shape = new btConeShape(btScalar(dimensions.x), btScalar(dimensions.y)); //TODO Radius and height
	}
	if (type == complexMesh) {
		//shape = new btTriangleMeshShape(btTriangleMeshShape obj);
		//data = new btTriangleMeshShapeData();
	}
	
	SetBulletPhysicsParameters();
}

PhysicsObject::~PhysicsObject()	{

}

void PhysicsObject::SetBulletPhysicsParameters()
{
	btTransform btTransform;
	btQuaternion orient = btQuaternion(transform->GetLocalOrientation().x, transform->GetLocalOrientation().y, transform->GetLocalOrientation().z, transform->GetLocalOrientation().w);

	btTransform.setIdentity();
	btTransform.setOrigin(btVector3(transform->GetLocalPosition().x, transform->GetLocalPosition().y, transform->GetLocalPosition().z));
	btTransform.setRotation(orient);

	btScalar btMass(mass);
	bool isDynamic = (btMass != 0.0f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(btMass, localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(btTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(btMass, myMotionState, shape, localInertia);

	body = new btRigidBody(rbInfo);
	
	//body->setLinearVelocity(btVector3(10, 100, 0));
	//body->setAngularVelocity(btVector3(0, 10, 0));

	body->applyImpulse(btVector3(-1000, 1000, 0), btVector3(0, -5, 0));

	body->setFriction(friction);
	body->setRestitution(restitution);
	body->setRollingFriction(0.9);
	body->setSpinningFriction(0.3);
}

void PhysicsObject::ForceUpdateBulletWorldTransform(btTransform &temp)
{
	body->getMotionState()->setWorldTransform(temp);
	body->setWorldTransform(temp);
}

void PhysicsObject::ForceUpdateWorldPosition(Vector3 pos)
{
	btTransform temp;
	body->getMotionState()->getWorldTransform(temp);
	temp.setOrigin(btVector3(pos.x, pos.y, pos.z));
	ForceUpdateBulletWorldTransform(temp);
}

void PhysicsObject::ForceUpdateWorldPositionWithTransform(Vector3 pos)
{
	transform->SetWorldPosition(pos);
	ForceUpdateWorldPosition(pos);
}

void PhysicsObject::ForceUpdateLocalRotation(Quaternion qt)
{
	btTransform temp;
	body->getMotionState()->getWorldTransform(temp);
	temp.setRotation(btQuaternion(qt.x,qt.y,qt.z,qt.w));
	ForceUpdateBulletWorldTransform(temp);
}

void PhysicsObject::ForceUpdateLocalRotationWithTransform(Quaternion qt)
{
	transform->SetLocalOrientation(qt);
	ForceUpdateLocalRotation(qt);
}

void PhysicsObject::ForceUpdateScale(Vector3 scale)
{
	if (type == cube) {
		body->setCollisionShape(new btBoxShape(btVector3(scale.x, scale.y, scale.z)));
	}
	if (type == sphere) {
		body->setCollisionShape(new btSphereShape(scale.x));
	}
	if (type == cylinder) {
		body->setCollisionShape(new btCylinderShape(btVector3(btScalar(scale.x), btScalar(scale.y), btScalar(scale.z))));
	}
	if (type == cone) {
		body->setCollisionShape(new btConeShape(btScalar(scale.x), btScalar(scale.y)));
	}
}

void PhysicsObject::ForceUpdateScaleWithTransform(Vector3 scale)
{
	transform->SetLocalScale(scale);
	ForceUpdateScale(scale);
}
