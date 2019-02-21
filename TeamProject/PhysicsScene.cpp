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
InputManager * PhysicsScene::inputManager = nullptr;

PhysicsScene::PhysicsScene() : Scene() {
  Window::GetWindow()->ShowOSPointer(false);
  Window::GetWindow()->LockMouseToWindow(true);

  ResetWorld();
  debugMenu = DebugMenu();
  console = Console();

  GameObject::SetGameWorld(world);
  if (!inputManager) { inputManager = new InputManager(); }//Static guy initializations

}

void PhysicsScene::ResetWorld() {
  world->ClearAndErase();

  AddCubeToWorld(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0, 1.0f, 1.0f); //TODO Do these need to be deleted in destructor?!?!?!
  //AddCubeToWorld(Vector3(0, 100, 0), Quaternion::AxisAngleToQuaternion(Vector3(0, 1, 0), 45), Vector3(2, 2, 2), 1);
  //bestcube = AddCubeToWorld(Vector3(3.5, 130, 1), Quaternion::AxisAngleToQuaternion(Vector3(1, 1, 1), 45), Vector3(20, 20, 20), 10000);
  //AddSphereToWorld(Vector3(40, 100, 20), 1, 1);
  //AddSphereToWorld(Vector3(41, 130, 20), 1, 1);
 // InitMixedGridWorld(Vector3(47, 230, 20), 50, 40, 20, 20);
  // AddCylinderToWorld(Vector3(0, 10, 0), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(2, 2, 2), 1);
  // AddConeToWorld(Vector3(Vector3(0, 10, 0), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(2, 2, 2), 1);

   //Player
  auto player = AddCubeToWorld(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);
  player->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
  player->AddScript((ScriptObject*)new Player(player));
  player->SetTag(LayerAndTag::Tags::Player);
  player->GetPhysicsObject()->GetRigidbody()->setActivationState(DISABLE_DEACTIVATION);
  world->GetMainCamera()->GetScript<CameraControl*>()->SetPlayer(player);

  auto resource1 = AddCubeToWorld(Vector3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f);
  resource1->SetName("Resource 1");
  resource1->AddScript((ScriptObject*)new Resource(resource1));

  auto resource2 = AddCubeToWorld(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f);
  resource2->SetName("Resource 2");
  resource2->AddScript((ScriptObject*)new Resource(resource2));
}

PhysicsScene::~PhysicsScene() {
	delete tempTex;
}

void PhysicsScene::UpdateKeys() {
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_O)) {
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->activate();
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->setLinearVelocity(btVector3(0, 10, 0));
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_J)) {
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->activate();
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->setLinearVelocity(btVector3(-10, 0, 0));
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_K)) {
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->activate();
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->setLinearVelocity(btVector3(10, 0, 0));
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_N)) {
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->activate();
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->setLinearVelocity(btVector3(0, 0, 10));
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_M)) {
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->activate();
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[2])->setLinearVelocity(btVector3(0, 0, -10));
	}
}

void PhysicsScene::UpdateGame(float dt) {
//  world->GetMainCamera()->UpdateCamera(dt);
  world->UpdateWorld(dt);

  UpdateKeys();
  renderer->Update(dt);
  physics->Update(dt);
  //bestcube->GetPhysicsObject()->GetRigidbody()->applyImpulse(btVector3(-1, 10000, 10), btVector3(0, -10, 0));
  //bestcube->GetPhysicsObject()->SetLinearVelocity(Vector3(100, 0, 0));
  //bestcube->GetPhysicsObject()->SetAngularVelocity(Vector3(0, 10, 0));
  //bestcube->GetPhysicsObject()->ApplyForce(Vector3(100000, 0, 10), Vector3(0, -10, 0));
  //bestcube->GetPhysicsObject()->ApplyTorque(Vector3(0, 10000000, 0));

  Debug::FlushRenderables();
  debugMenu.Update(dt, renderer);
  console.Update();

  renderer->Render();
}

void PhysicsScene::DebugScene(float dt) {
}
