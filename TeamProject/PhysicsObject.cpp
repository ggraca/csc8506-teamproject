#include "PhysicsObject.h"
#include "../TeamProject/Transform.h"
#include "../TeamProject/Scene.h"

#include "../../BulletWorldImporter/btBulletWorldImporter.h"
#include "../../BulletFileLoader/btBulletFile.h"

using namespace NCL;
using namespace CSC8503;

PhysicsObject::PhysicsObject(Transform* parentTransform, ShapeType type, float mass, float restitution, float friction, OBJGeometry* mesh)	{
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

		


		btTriangleMesh* triangleMesh = new btTriangleMesh();

	/*	btConvexHullShape* shape1 = new btConvexHullShape();*/
		

	//	cout << "Mesh: " << gameObject->GetComponent<RenderObject*>()->GetMesh()->GetPositionData()[1] << endl;

	//	cout << "Mesh: " << gameObject->GetChildrenOfObject(gameObject)[0]->GetComponent<RenderObject*>()->GetMesh()->GetPositionData()[0] << endl;

		//cout << "Mesh: " << mesh->GetChildren()[0]->GetPositionData().size() << endl;
		//cout << "Mesh: " << mesh->GetChildren()[0]->GetPositionData()[0] << endl;
		//cout << "Mesh: " << mesh->GetChildren()[0]->GetPositionData()[1] << endl;
		//cout << "Mesh: " << mesh->GetChildren()[0]->GetPositionData()[22487] << endl;

		Vector3 vert1, vert2, vert3;
		btVector3 vertex1, vertex2, vertex3;

		/*vertex1 = btVector3(-1000, 0, 0);
		vertex2 = btVector3(1000, 0, 0);
		vertex3 = btVector3(0, 100, 0);
		triangleMesh->addTriangle(vertex1, vertex2, vertex3);*/
		
		
		for (int i = 0; i < mesh->GetChildren()[0]->GetPositionData().size(); ) {
			vert1 = mesh->GetChildren()[0]->GetPositionData()[i];
			vert2 = mesh->GetChildren()[0]->GetPositionData()[i+1];
			vert3 = mesh->GetChildren()[0]->GetPositionData()[i+2];

			cout << vert1 << ' ' << vert2 << ' ' << vert3 << endl;
			

			vertex1 = btVector3(vert1.x * dimensions.x, vert1.y * dimensions.y, vert1.z * dimensions.z);
			vertex2 = btVector3(vert2.x * dimensions.x, vert2.y * dimensions.y, vert2.z * dimensions.z);
			vertex3 = btVector3(vert3.x * dimensions.x, vert3.y * dimensions.y, vert3.z * dimensions.z);

			/*shape1->addPoint(vertex1);
			shape1->addPoint(vertex2);
			shape1->addPoint(vertex3);*/

			/*vertex1 = btVector3(vert1.x, vert1.y, vert1.z);
			vertex2 = btVector3(vert2.x, vert2.y, vert2.z);
			vertex3 = btVector3(vert3.x, vert3.y, vert3.z);*/
			triangleMesh->addTriangle(vertex1, vertex2, vertex3);
			i += 3;
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

		/*btCollisionShape**/ shape = new btBvhTriangleMeshShape(triangleMesh, true);
		btVector3 min, max;
		const btTransform t;
		shape->getAabb(t, min, max);

		Vector3 mini = Vector3(min.getX(), min.getY(), min.getZ());
		Vector3 maxi = Vector3(max.getX(), max.getY(), max.getZ());
		cout << mini << ' ' << maxi << endl;

		//shape = new btBoxShape(btVector3(btScalar(max.getX() - min.getX()), btScalar(max.getY() - min.getY()), btScalar(max.getZ() - min.getZ())));
	
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

	//	shape = new btBoxShape(btVector3(btScalar(100), btScalar(200), btScalar(100)));

	//	shape = new btCompoundShape();



	//	btBulletWorldImporter loader;
		/*loader.setVerboseMode(true);
		if (!loader.loadFile("C:\\Users\\b7053261\\GitHub\\csc8506-teamproject\\Assets\\Meshes\\back_wall.bcs")) cout << "shoot!";
		if (loader.getNumCollisionShapes() > 0) shape = loader.getCollisionShapeByIndex(0);*/

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

//btCollisionShape* PhysicsObject::Load(const char* filename, bool verbose) {
//	btBulletWorldImporter loader(0);//don't store info into the world
//	loader.setVerboseMode(verbose);
//	if (!loader.loadFile(filename)) return nullptr;
//	btCollisionShape* shape = nullptr;
//	if (loader.getNumCollisionShapes() > 0) shape = loader.getCollisionShapeByIndex(0);
//	// Hope there are no leaks.
//	return shape;
//}

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
