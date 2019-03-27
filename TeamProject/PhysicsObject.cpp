#include "PhysicsObject.h"
#include "../TeamProject/Transform.h"
#include "../TeamProject/Scene.h"
#include <fstream>
#include <string>
#include <algorithm>

using namespace NCL;
using namespace CSC8503;

PhysicsObject::PhysicsObject(Transform* parentTransform, ShapeType type, float mass, float restitution, float friction, string objFile, bool boxCollider) {
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
		objFile.erase(objFile.length() - 3);
		objFile = objFile + "txt";

		ifstream boxData(Assets::MESHDIR + objFile);
		if (boxData) {
			compound = new btCompoundShape(); //TODO Where is shape deleted?
			string a;
			int numBoxes;
			float elem;

			boxData >> a;
			istringstream iss(a);
			iss >> numBoxes;
			vector<float> data;
			for (int i = 0; i < numBoxes * 6; i++) {
				boxData >> a;
				a.erase(remove(a.begin(), a.end(), ','), a.end());
				float elem = (float)strtof(a.c_str(), NULL);
				cout << elem << endl;
				data.push_back(elem);
			}
			boxData.close();

			btTransform t;
			t.setIdentity();
			for (int i = 0; i < numBoxes * 6;) {
				boxShape = new btBoxShape(btVector3(0.5 * dimensions.x * data[i], 0.5 * dimensions.y * data[i + 1], 0.5 * dimensions.z * data[i + 2])); //TODO Where is this deleted?
				t.setOrigin(btVector3(dimensions.x * data[i + 3], dimensions.y * data[i + 4], dimensions.z * data[i + 5]));
				compound->addChildShape(t, boxShape);
				i += 6;
			}
			shape = compound;
		}
		else {
			btTriangleMesh* triangleMesh = new btTriangleMesh(); //TODO How to delete this properly?
			OBJGeometry* mesh = Assets::AssetManager::LoadOBJ(objFile);
			if (mesh->GetChildren().size() == 1) {
				Vector3 vert1, vert2, vert3;
				btVector3 vertex1, vertex2, vertex3;
				cout << mesh->GetChildren().size() << endl;  
				cout << mesh->GetChildren()[0]->GetPositionData().size() << endl;
				for (int i = 0; i < mesh->GetChildren()[0]->GetPositionData().size(); ) {
					vert1 = mesh->GetChildren()[0]->GetPositionData()[i];
					vert2 = mesh->GetChildren()[0]->GetPositionData()[i + 1];
					vert3 = mesh->GetChildren()[0]->GetPositionData()[i + 2];

//					cout << vert1 << ' ' << vert2 << ' ' << vert3 << endl; //TODO Delete this once fully tested!

					vertex1 = btVector3(vert1.x * dimensions.x, vert1.y * dimensions.y, vert1.z * dimensions.z);
					vertex2 = btVector3(vert2.x * dimensions.x, vert2.y * dimensions.y, vert2.z * dimensions.z);
					vertex3 = btVector3(vert3.x * dimensions.x, vert3.y * dimensions.y, vert3.z * dimensions.z);

					triangleMesh->addTriangle(vertex1, vertex2, vertex3);
					i += 6; //obj file reader repeats every triplet of vertices
				}
				cout << "Triangles: " << triangleMesh->getNumTriangles() << endl; //TODO Delete this once fully tested!
				if (boxCollider) { //If true, create a box collision shape from minimum and maximum extents of mesh
					cout << "yes" << endl;
					meshShape = new btBvhTriangleMeshShape(triangleMesh, true); //TODO How to delete this properly?
					btVector3 min, max;
					btTransform t1;
					t1.setIdentity();
					meshShape->getAabb(t1, min, max);
					shape = new btBoxShape(btVector3(0.5 * btScalar(max.getX() - min.getX()), btScalar(max.getY() - min.getY()), 0.5 * btScalar(max.getZ() - min.getZ()))); //y-component of collision volume should really be halved, then translated up by half its height, but for static geometry this makes no difference!
				}
				else
				{
					shape = new btBvhTriangleMeshShape(triangleMesh, true);
				}
			}
		}
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
	
	body->setFriction(friction);
	body->setRestitution(restitution);
	body->setRollingFriction(0.9);
	body->setSpinningFriction(0.3);

	/*if (!isDynamic)
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}*/
	
}
