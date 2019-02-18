#include "PhysicsScene.h"
#include "GameWorld.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/TextureLoader.h"

#include "../Common/Assets.h"
#include <fstream>

using namespace NCL;
using namespace CSC8503;

PhysicsScene::PhysicsScene() : Scene() {
  Window::GetWindow()->ShowOSPointer(false);
  Window::GetWindow()->LockMouseToWindow(true);

  ResetWorld();
  debugMenu = DebugMenu();
  console = Console();
}

void PhysicsScene::ResetWorld() {
  world->ClearAndErase();

  //AddCubeToWorld(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0); //TODO Do these need to be deleted in destructor?!?!?!
  //AddCubeToWorld(Vector3(0, 100, 0), Quaternion::AxisAngleToQuaternion(Vector3(0, 1, 0), 45), Vector3(2, 2, 2), 1);
  //AddCubeToWorld(Vector3(3.5, 130, 1), Quaternion::AxisAngleToQuaternion(Vector3(1, 1, 1), 45), Vector3(20, 20, 20), 0.05);
  //AddSphereToWorld(Vector3(40, 100, 20), 1, 1);
  //AddSphereToWorld(Vector3(41, 130, 20), 1, 1);
  //InitMixedGridWorld(Vector3(47, 230, 20), 10, 10, 20, 20);

  PhysicsObject physOb(cuboid, Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0, 1, 0);
  AddObjectToWorld(physOb);
  PhysicsObject physOb1(cuboid, Vector3(3.5, 130, 1), Quaternion::AxisAngleToQuaternion(Vector3(1, 1, 1), 45), Vector3(20, 20, 20), 20, 0.8, 0);
  AddObjectToWorld(physOb);
  PhysicsObject physOb2(sphere, Vector3(40, 100, 20), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(1, 1, 1), 1, (rand() % 100) / (float)100, (rand() % 100) / (float)100);
  AddObjectToWorld(physOb);
  OOInitMixedGridWorld(Vector3(47, 230, 20), 50, 40, 20, 20);

  // AddCylinderToWorld(Vector3(47, 230, 20), Vector3(2, 2, 2), 1);
}

PhysicsScene::~PhysicsScene() {
}

void PhysicsScene::UpdateKeys() {
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_O)) {
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->activate();
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->setLinearVelocity(btVector3(0, 100, 0));
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_J)) {
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->activate();
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->setLinearVelocity(btVector3(-100, 0, 0));
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_K)) {
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->activate();
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->setLinearVelocity(btVector3(100, 0, 0));
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_N)) {
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->activate();
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->setLinearVelocity(btVector3(0, 0, 100));
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_M)) {
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->activate();
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->setLinearVelocity(btVector3(0, 0, -100));
	}
}

void PhysicsScene::UpdateGame(float dt) {
  world->GetMainCamera()->UpdateCamera(dt);
  world->UpdateWorld(dt);

  UpdateKeys();
  renderer->Update(dt);
  physics->Update(dt);

  Debug::FlushRenderables();
  debugMenu.Update(dt, renderer);
  console.Update();

  renderer->Render();
}

void PhysicsScene::DebugScene(float dt) {
}
