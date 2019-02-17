#include "PhysicsObject.h"
#include "../TeamProject/Transform.h"

using namespace NCL;
using namespace CSC8503;

PhysicsObject::PhysicsObject(Transform* parentTransform)	{
	transform	= parentTransform;
	dimensions = Vector3(1, 1, 1);
	orientation = Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0);
	position = Vector3(0, 0, 0);
	mass = 1.0f;
	restitution = 0.8f;
	friction = 0.8f;
	dimensions = Vector3(1, 1, 1);
}

PhysicsObject::~PhysicsObject()	{

}

