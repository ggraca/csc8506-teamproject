#include "PhysicsObject.h"

//using namespace NCL;
//using namespace CSC8503;

PhysicsObject::PhysicsObject(NCL::CSC8503::Transform* parentTransform)	{
	dimensions = Vector3(1, 1, 1);
	orientation = Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0);
	position = Vector3(0, 0, 0);
	mass = 1.0f;
	restitution	= 0.8f;
	friction	= 0.8f;
	dimensions = Vector3(1, 1, 1);

	btCollisionShape* Shape = new btBoxShape(btVector3(btScalar(dimensions.x), btScalar(dimensions.y), btScalar(dimensions.z)));
	scene->Scene::SetBulletPhysicsParameters(Shape, position, mass, restitution, friction, orientation);
}

PhysicsObject::~PhysicsObject()	{

}



