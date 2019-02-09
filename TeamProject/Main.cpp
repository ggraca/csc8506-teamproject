#include <time.h>

#include "../Common/Window.h"
#include "ExampleScene.h"

using namespace NCL;
using namespace CSC8503;

#include "..\GameTechCommon\BulletPhysics.h"

BulletPhysics* bulletPhysics = new BulletPhysics();

//void SetBulletPhysicsParameters(btCollisionShape* Shape, btScalar& positionx, btScalar& positiony, btScalar& positionz, float inverseMass)
//{
//	//bulletPhysics->collisionShapes.push_back(Shape);
//	//btTransform Transform;
//	//Transform.setIdentity();
//	//Transform.setOrigin(btVector3(positionx, positiony, positionz)); //TODO Cast position vector?
//	////Transform.setOrigin(btVector3(position)); //TODO Cast position vector?
//	////Transform.setOrigin(position); //TODO Cast position vector?
//	//btScalar mass(inverseMass);
//	//bool isDynamic = (mass != 0.0f);
//	//btVector3 localInertia(0, 0, 0);
//	//if (isDynamic)
//	//	Shape->calculateLocalInertia(mass, localInertia);
//	//btDefaultMotionState* myMotionState = new btDefaultMotionState(Transform);
//	//btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, Shape, localInertia);
//	//btRigidBody* body = new btRigidBody(rbInfo);
//	//bulletPhysics->dynamicsWorld->addRigidBody(body);
//}

int main() {
	srand(time(NULL));

	//btCollisionShape* Shape1 = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
	//bulletPhysics->collisionShapes.push_back(Shape1);
	//btTransform Transform;
	//Transform.setIdentity();
	//Transform.setOrigin(btVector3(0, -56, 0)); //TODO Cast position vector?
	////Transform.setOrigin(btVector3(position)); //TODO Cast position vector?
	////Transform.setOrigin(position); //TODO Cast position vector?
	//btScalar mass(0);
	//bool isDynamic = (mass != 0.0f);
	//btVector3 localInertia(0, 0, 0);
	//if (isDynamic)
	//	Shape1->calculateLocalInertia(mass, localInertia);
	//btDefaultMotionState* myMotionState = new btDefaultMotionState(Transform);
	//btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, Shape1, localInertia);
	//btRigidBody* body = new btRigidBody(rbInfo);
	//bulletPhysics->dynamicsWorld->addRigidBody(body);



	//btCollisionShape* Shape2 = new btSphereShape(btScalar(1));
	//
	//bulletPhysics->collisionShapes.push_back(Shape2);
	//btTransform Transform1;
	//Transform1.setIdentity();
	//Transform1.setOrigin(btVector3(2, 10, 0)); //TODO Cast position vector?
	////Transform.setOrigin(btVector3(position)); //TODO Cast position vector?
	////Transform.setOrigin(position); //TODO Cast position vector?
	//btScalar mass1(1);
	//bool isDynamic1 = (mass1 != 0.0f);
	//btVector3 localInertia1(0, 0, 0);
	//if (isDynamic1)
	//	Shape1->calculateLocalInertia(mass1, localInertia);
	//btDefaultMotionState* myMotionState1 = new btDefaultMotionState(Transform1);
	//btRigidBody::btRigidBodyConstructionInfo rbInfo1(mass1, myMotionState1, Shape2, localInertia1);
	//btRigidBody* body1 = new btRigidBody(rbInfo1);
	//bulletPhysics->dynamicsWorld->addRigidBody(body1);

	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

		bulletPhysics->collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		//bulletPhysics->dynamicsWorld->addRigidBody(body);
	}

	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		bulletPhysics->collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(2, 10, 0));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//bulletPhysics->dynamicsWorld->addRigidBody(body);
	}

  /// Do some simulation

  ///-----stepsimulation_start-----
  //for (int i = 0; i < 150; i++)
  //{
	 // bulletPhysics->dynamicsWorld->stepSimulation(1.f / 60.f, 10);

	 // //print positions of all objects
	 // for (int j = bulletPhysics->dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	 // {
		//  btCollisionObject* obj = bulletPhysics->dynamicsWorld->getCollisionObjectArray()[j];
		//  btRigidBody* body = btRigidBody::upcast(obj);
		//  btTransform trans;
		//  if (body && body->getMotionState())
		//  {
		//	  body->getMotionState()->getWorldTransform(trans);
		//  }
		//  else
		//  {
		//	  trans = obj->getWorldTransform();
		//  }
		//  printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	 // }
  //}
  //system("pause");

  delete bulletPhysics;



	Window*w = Window::CreateGameWindow("Team Project!", 1280, 720);
	if (!w->HasInitialised()) return -1;

	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);
    w->ShowConsole(true);

	ExampleScene* g = new ExampleScene();

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float dt = w->GetTimer()->GetTimeDelta() / 1000.0f;

		if (dt > 1.0f) {
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}

		g->UpdateGame(dt);
	}

	delete g;
	Window::DestroyGameWindow();
}
