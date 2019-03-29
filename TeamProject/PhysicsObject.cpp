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
		string simplifiedMeshFile = Assets::MESHDIR + "x_" + objFile;
		objFile.erase(objFile.length() - 3);
		objFile = objFile + "txt";

		ifstream boxData(Assets::MESHDIR + objFile);
		if (boxData) { //If a text file, containing cuboid dimensions and positions to make compound collision shape, exists with same name as OBJ file
			compound = new btCompoundShape();
			string a;
			int numBoxes;
			boxData >> a;
			if (a == "cylinder") {
				boxData >> a;
				a.erase(remove(a.begin(), a.end(), ','), a.end());
				float radius = (float)strtof(a.c_str(), NULL);
				boxData >> a;			
				float height = (float)strtof(a.c_str(), NULL);		
				boxData.close();
				shape = new btCylinderShape(btVector3(btScalar(dimensions.x * radius), btScalar(dimensions.y * height), btScalar(dimensions.x * radius)));
			}
			else
			{
				istringstream iss(a);
				iss >> numBoxes;
				vector<float> data;
				for (int i = 0; i < numBoxes * 6; i++) {
					boxData >> a;
					a.erase(remove(a.begin(), a.end(), ','), a.end());
					float elem = (float)strtof(a.c_str(), NULL);
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
		}
		else {
			btTriangleMesh* triangleMesh = new btTriangleMesh();
			ifstream simplifiedMeshData(simplifiedMeshFile);                  //Simplified mesh files can be created with vhacd_vs2010.exe, located in bullet3-master\bin, drastically reducing the number of vertices
			string fileName = objFile;										  //in an OBJ file, but maintaining almost the same volume, making such files ideal for feeding into Bullet to create precise collision
			if (simplifiedMeshData) {										  //volumes. Such files must be prefixed with an "x_", and a couple of examples should be in the Meshes folder. Unfortunately, these collision
				fileName = "x_" + objFile;									  //volumes don't work well if there are any direct interactions with the player avatar, since the decision was made to manipulate its 
			}																  //position directly, temporarily overriding Bullet Physics and thus its collision response. Therefore, manually creating collision volumes 
			OBJGeometry* mesh = Assets::AssetManager::LoadOBJ(fileName);	  //from text files was deemed the only method to achieve completely reliable and reasonably precise results. Thus, this is effectively 
			Vector3 vert1, vert2, vert3;									  //legacy code, but is retained because quite a large amount of work and testing went into it!	
			btVector3 vertex1, vertex2, vertex3;
			for (int j = 0; j < mesh->GetChildren().size(); j++) {
				for (int i = 0; i < (int)mesh->GetChildren()[j]->GetPositionData().size(); ) {
					vert1 = mesh->GetChildren()[j]->GetPositionData()[i];
					vert2 = mesh->GetChildren()[j]->GetPositionData()[i + 1];
					vert3 = mesh->GetChildren()[j]->GetPositionData()[i + 2];

					vertex1 = btVector3(vert1.x * dimensions.x, vert1.y * dimensions.y, vert1.z * dimensions.z);
					vertex2 = btVector3(vert2.x * dimensions.x, vert2.y * dimensions.y, vert2.z * dimensions.z);
					vertex3 = btVector3(vert3.x * dimensions.x, vert3.y * dimensions.y, vert3.z * dimensions.z);

					triangleMesh->addTriangle(vertex1, vertex2, vertex3);
					i += 6; //obj file reader repeats every triplet of vertices
				}
			}
			if (boxCollider) { //If true, create a box collision shape from minimum and maximum extents of mesh
				meshShape = new btBvhTriangleMeshShape(triangleMesh, true);
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
}
