#include "PhysicsObject.h"
#include "../TeamProject/Transform.h"
#include "../TeamProject/Scene.h"

using namespace NCL;
using namespace CSC8503;

PhysicsObject::PhysicsObject(Transform* parentTransform, ShapeType type, float mass, float restitution, float friction, OBJGeometry* mesh, bool boxCollider)	{
	transform = parentTransform;
	this->type = type;
	this->mass = mass;
	this->restitution = restitution;
	this->friction = friction;

	Vector3 dimensions = transform->GetLocalScale();
	
	if (type == cube) {
		shape = new btBoxShape(btVector3(btScalar(dimensions.x), btScalar(dimensions.y), btScalar(dimensions.z))); //TODO Where is shape deleted?
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

		//btTriangleMesh* triangleMesh = new btTriangleMesh(); //TODO How to delete this properly?

		//cout << "Num of children: " << mesh->GetChildren().size() << endl;

		//if (mesh->GetChildren().size() > 1) {
		//	compound = new btCompoundShape();
		//}

		compound = new btCompoundShape();
	
	/*	btConvexHullShape* shape1 = new btConvexHullShape();*/
	//	cout << "Mesh: " << gameObject->GetComponent<RenderObject*>()->GetMesh()->GetPositionData()[1] << endl;
	//	cout << "Mesh: " << gameObject->GetChildrenOfObject(gameObject)[0]->GetComponent<RenderObject*>()->GetMesh()->GetPositionData()[0] << endl;
	//  cout << "Mesh: " << mesh->GetChildren()[0]->GetPositionData().size() << endl;
	//  cout << "Mesh: " << mesh->GetChildren()[0]->GetPositionData()[0] << endl;
	//  cout << "Mesh: " << mesh->GetChildren()[0]->GetPositionData()[1] << endl;
	//  cout << "Mesh: " << mesh->GetChildren()[0]->GetPositionData()[22487] << endl;
		int x = 0;

		//Vector3 vert1, vert2, vert3;
		//btVector3 vertex1, vertex2, vertex3;
		//for (int j = 0; j < mesh->GetChildren().size(); j++) {
		//	x++;
		//	for (int i = 0; i < mesh->GetChildren()[j]->GetPositionData().size(); ) {
		//		vert1 = mesh->GetChildren()[j]->GetPositionData()[i];
		//		vert2 = mesh->GetChildren()[j]->GetPositionData()[i + 1];
		//		vert3 = mesh->GetChildren()[j]->GetPositionData()[i + 2];

		//		cout << vert1 << ' ' << vert2 << ' ' << vert3 << endl;

		//		vertex1 = btVector3(vert1.x * dimensions.x, vert1.y * dimensions.y, vert1.z * dimensions.z);
		//		vertex2 = btVector3(vert2.x * dimensions.x, vert2.y * dimensions.y, vert2.z * dimensions.z);
		//		vertex3 = btVector3(vert3.x * dimensions.x, vert3.y * dimensions.y, vert3.z * dimensions.z);

		//		triangleMesh->addTriangle(vertex1, vertex2, vertex3);
		//		i += 6; //obj file reader repeats every triplet of vertices once
		//	}
		////	cout << "Triangles: " << triangleMesh->getNumTriangles() << endl;
		////	cout << "Num of children: " << mesh->GetChildren().size() << endl;

		//	if (boxCollider) {
		//		meshShape = new btBvhTriangleMeshShape(triangleMesh, true);

		//		btVector3 min, max;

		//		if (x == 1) {
		//		    min = btVector3(-23.234095, 0.000027, -24.929432);
		//			max = btVector3(76.765907, 19.714027, -44.929432);
		//		}
		//		else {
		//			btTransform t1;
		//			t1.setIdentity();
		//			meshShape->getAabb(t1, min, max);
		//		}
		//		
		//		boxShape = new btBoxShape(btVector3(0.5 * btScalar(max.getX() - min.getX()), btScalar(max.getY() - min.getY()), 0.5 * btScalar(max.getZ() - min.getZ())));

		//		if (mesh->GetChildren().size() > 1) {
		//			btTransform t2;
		//			t2.setIdentity();
		//			compound->addChildShape(t2, boxShape);
		//		}
		//	}
		//	else
		//	{
		//		shape = new btBvhTriangleMeshShape(triangleMesh, true);
		//	}	
		//}
		//
		//if (boxCollider) {
		//	if (mesh->GetChildren().size() > 1) {
		//		shape = compound;
		//	}
		//	else {
		//		shape = boxShape;
		//	}
		//}
	/*	btVector3 min = btVector3(-23.234095, 0.000027, -24.929432);
		btVector3 max = btVector3(76.765907, 19.714027, -44.929432);*/
//		shape = new btBoxShape(btVector3(0.5 * btScalar(max.getX() - min.getX()), btScalar(max.getY() - min.getY()), 0.5 * btScalar(max.getZ() - min.getZ()))); //TODO Where is shape deleted?

		btVector3 min = btVector3(-250, 0.000000, -25.090006);
		btVector3 max = btVector3(250, 138.298004, 24.909994);
		boxShape = new btBoxShape(btVector3(0.5 * btScalar(max.getX() - min.getX()), btScalar(max.getY() - min.getY()), 0.5 * btScalar(max.getZ() - min.getZ()))); //TODO Where is shape deleted?
		btTransform t2;
		t2.setIdentity();
		t2.setOrigin(btVector3(30, 0, 0));
		compound->addChildShape(t2, boxShape);


		min = btVector3(-250.526226, 0.000000, 18.910019);
		max = btVector3(250.473774, 12.000000, 23.910019);
		boxShape = new btBoxShape(btVector3(0.5 * btScalar(max.getX() - min.getX()), btScalar(max.getY() - min.getY()), 0.5 * btScalar(max.getZ() - min.getZ()))); //TODO Where is shape deleted?
		t2.setOrigin(btVector3(30, 138.298004, 21.4));
		compound->addChildShape(t2, boxShape);



		min = btVector3(-250.526226, 0.000000, 18.910019);
		max = btVector3(-40, 12.000000, 23.910019);
		boxShape = new btBoxShape(btVector3(0.5 * btScalar(max.getX() - min.getX()), btScalar(max.getY() - min.getY()), 0.5 * btScalar(max.getZ() - min.getZ()))); //TODO Where is shape deleted?
		t2.setOrigin(btVector3(-130, 138.298004, -20.4));
		compound->addChildShape(t2, boxShape);

		min = btVector3(50.526226, 0.000000, 18.910019);
		max = btVector3(250, 12.000000, 23.910019);
		boxShape = new btBoxShape(btVector3(0.5 * btScalar(max.getX() - min.getX()), btScalar(max.getY() - min.getY()), 0.5 * btScalar(max.getZ() - min.getZ()))); //TODO Where is shape deleted?
		t2.setOrigin(btVector3(180, 138.298004, -20.4));
		compound->addChildShape(t2, boxShape);



		min = btVector3(-23.234095, 0.000000, -44.929432);
		max = btVector3(76.765907, 19.714027, -24.929432); 
		boxShape = new btBoxShape(btVector3(0.5 * btScalar(max.getX() - min.getX()), btScalar(max.getY() - min.getY()), 0.5 * btScalar(max.getZ() - min.getZ()))); //TODO Where is shape deleted?
		t2.setOrigin(btVector3(24.7, 0, -155));
		compound->addChildShape(t2, boxShape);

	
		t2.setOrigin(btVector3(24.7, 19.714027, -135));
		compound->addChildShape(t2, boxShape);

		t2.setOrigin(btVector3(24.7, 2 * 19.714027, -115));
		compound->addChildShape(t2, boxShape);

		t2.setOrigin(btVector3(24.7, 3 * 19.714027, -95));
		compound->addChildShape(t2, boxShape);

		t2.setOrigin(btVector3(24.7, 5 * 19.714027, -55));
		compound->addChildShape(t2, boxShape);

		t2.setOrigin(btVector3(24.7, 6 * 19.714027, -35));
		compound->addChildShape(t2, boxShape);

		min = btVector3(-23.234095, 0.000000, -44.929432);
		max = btVector3(76.765907, 5 * 19.714027, -24.929432);
		boxShape = new btBoxShape(btVector3(0.5 * btScalar(max.getX() - min.getX()), btScalar(max.getY() - min.getY()), 0.5 * btScalar(max.getZ() - min.getZ()))); //TODO Where is shape deleted?
		t2.setOrigin(btVector3(24.7, 0, -75));
		compound->addChildShape(t2, boxShape);

		/*shape = new btTriangleMeshShape(btTriangleMeshShape obj);
		data = new btTriangleMeshShapeData();*/

		////Compound shape method:
		//btCylinderShape * cylinder1 = new btCylinderShape(btVector3(1, 1.7, 1));
		//btCylinderShape * cylinder2 = new btCylinderShape(btVector3(0.5, 1, 0.5));
		//btCompoundShape * bolw = new btCompoundShape();
		//bolw->addChildShape(t, cylinder1);
		//t.setIdentity();
		//t.setOrigin(btVector3(x, y + 2.7, z));
		//bolw->addChildShape(t, cylinder2);
		//btVector3 inertia(0, 0, 0);
		//btScalar masses[2] = { mass,mass / 2 };
		//bolw->calculatePrincipalAxisTransform(masses, t, inertia);
		//t.setIdentity();


		shape = compound;
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
