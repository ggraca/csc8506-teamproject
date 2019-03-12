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

PhysicsObject::PhysicsObject(Transform* parentTransform, ShapeType type, float mass, float restitution, float friction, OBJGeometry* mesh)	{
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
		shape = new btCylinderShape(btVector3(btScalar(0.5 * dimensions.x), btScalar(0.5 * dimensions.y), btScalar(0.5 * dimensions.z)));
	}
	if (type == cone) {
		shape = new btConeShape(btScalar(0.5 * dimensions.x), btScalar(dimensions.y));
	}
	if (type == complexMesh) {
		btTriangleMesh* triangleMesh = new btTriangleMesh();
		

	//	cout << "Mesh: " << gameObject->GetComponent<RenderObject*>()->GetMesh()->GetPositionData()[1] << endl;

	//	cout << "Mesh: " << gameObject->GetChildrenOfObject(gameObject)[0]->GetComponent<RenderObject*>()->GetMesh()->GetPositionData()[0] << endl;

		//cout << "Mesh: " << mesh->GetChildren()[0]->GetPositionData().size() << endl;
		//cout << "Mesh: " << mesh->GetChildren()[0]->GetPositionData()[0] << endl;
		//cout << "Mesh: " << mesh->GetChildren()[0]->GetPositionData()[1] << endl;
		//cout << "Mesh: " << mesh->GetChildren()[0]->GetPositionData()[22487] << endl;

		Vector3 vert1, vert2, vert3;
		btVector3 vertex1, vertex2, vertex3;
		
		
		for (int i = 1200; i < 3756; ) {
			vert1 = mesh->GetChildren()[0]->GetPositionData()[i];
			vert2 = mesh->GetChildren()[0]->GetPositionData()[i+1];
			vert3 = mesh->GetChildren()[0]->GetPositionData()[i+2];

			cout << vert1 << ' ' << vert2 << ' ' << vert3 << endl;


			vertex1 = btVector3(vert1.x, vert1.y, vert1.z);
			vertex2 = btVector3(vert2.x, vert2.y, vert2.z);
			vertex3 = btVector3(vert3.x, vert3.y, vert3.z);
			triangleMesh->addTriangle(vertex1, vertex2, vertex3);
			i += 6;
		}
		cout << "Triangles: " << triangleMesh->getNumTriangles() << endl;

		/*btVector3 vertex1 = btVector3(0, 0, 0);
		btVector3 vertex2 = btVector3(1000, 0, 0);
		btVector3 vertex3 = btVector3(1000, 1000, 0);
		triangleMesh->addTriangle(vertex1, vertex2, vertex3);

		vertex1 = btVector3(0, 0, 0);
		vertex2 = btVector3(0, 1000, 0);
		vertex3 = btVector3(1000, 1000, 0);
		triangleMesh->addTriangle(vertex1, vertex2, vertex3);*/

		shape = new btBvhTriangleMeshShape(triangleMesh, true);

	
		/*shape = new btTriangleMeshShape(btTriangleMeshShape obj);
		data = new btTriangleMeshShapeData();*/
	}
	
	SetBulletPhysicsParameters();
}

PhysicsObject::~PhysicsObject()	{
	BulletPhysics* physics = gameObject->gameWorld->GetPhysics();

	delete body->getMotionState();
	physics->dynamicsWorld->removeCollisionObject(body);
	delete body;

	for (int i = 0; i < physics->collisionShapes.size(); i++) {
		if (physics->collisionShapes[i] == shape) {
			physics->collisionShapes[i] = 0;
			delete shape;
		}
	}
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

//	body->applyImpulse(btVector3(-1000, 1000, 0), btVector3(0, -5, 0));

	body->setFriction(friction);
	body->setRestitution(restitution);
	body->setRollingFriction(0.9);
	body->setSpinningFriction(0.3);
}
