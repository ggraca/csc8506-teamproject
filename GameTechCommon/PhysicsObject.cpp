#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(Transform* parentTransform)	{
	mass = 1.0f;
	restitution	= 0.8f;
	friction	= 0.8f;

	btCollisionShape* Shape = new btBoxShape(btVector3(btScalar(dimensions.x), btScalar(dimensions.y), btScalar(dimensions.z)));
	SetBulletPhysicsParameters(Shape, position, mass, restitution, friction, orient);
}

PhysicsObject::~PhysicsObject()	{

}



