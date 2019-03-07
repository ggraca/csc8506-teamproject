#include "PhysicsScene.h"
#include "GameWorld.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"

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

  GameObject::SetGameWorld(world);

}

void PhysicsScene::ResetWorld() {
  world->ClearAndErase();

  auto floor = AddCubeToWorld(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0, 1.0f, 1.0f); //TODO Do these need to be deleted in destructor?!?!?!
  floor->SetName("Floor");

   //Player
  auto player = AddCubeToWorld(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 0, 0.2f, 0.4f);
  player->AddScript((ScriptObject*)new Player(player));
  player->SetTag(LayerAndTag::Tags::Player);
  player->GetPhysicsObject()->GetRigidbody()->setActivationState(DISABLE_DEACTIVATION);
  player->GetRenderObject()->GetMaterial()->SetColour(Vector4(1, 0, 0, 1));
  player->SetLayer(LayerAndTag::ObjectLayer::UI);
  world->GetMainCamera()->GetScript<CameraControl*>()->SetPlayer(player);

  auto resource1 = AddCubeToWorld(Vector3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f);
  resource1->SetName("Resource 1");
  resource1->AddScript((ScriptObject*)new Resource(resource1));

  auto resource2 = AddCubeToWorld(Vector3(120, 300, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 0, 0.2f);
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

	//HUD TESTING BEGINS
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_U)) {
		renderer->WeaponState(2, true); //Hammer
		renderer->WeaponState(3, true); //Gun
		renderer->WeaponState(4, true); //Bomb
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_I)) {
		renderer->WeaponState(2, false); //Hammer
		renderer->WeaponState(3, false); //Gun
		renderer->WeaponState(4, false); //Bomb
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_Y))
	{
		hud.hp -= 5;
		renderer->health -= 0.05f;
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_T))
	{
		hud.hp = 100;
		renderer->health = 1.0f;
	}
	//HUD TESTING ENDS
}

const btCollisionObject* PhysicsScene::Raycast(const Vector3& Start, const Vector3& End, Vector3& NewEnd) {
	btVector3 btStart = btVector3(Start.x, Start.y, Start.z);
	btVector3 btEnd = btVector3(End.x, End.y, End.z);	
	btCollisionWorld::ClosestRayResultCallback RayCallback(btStart, btEnd);
	physics->dynamicsWorld->rayTest(btStart, btEnd, RayCallback);
	if (RayCallback.hasHit()) {
		btVector3 btNewEnd = RayCallback.m_hitPointWorld;
		NewEnd = Vector3(btNewEnd.getX(), btNewEnd.getY(), btNewEnd.getZ());
		return RayCallback.m_collisionObject;
	}
	return nullptr;
}

const btCollisionObject* PhysicsScene::RaycastPosDir(const Vector3& Pos, const Vector3& Dir, float t, Vector3& NewEnd) {
	btVector3 btStart = btVector3(Pos.x, Pos.y, Pos.z);
	Vector3 End = Pos + Vector3(t * Dir.x, t * Dir.y, t * Dir.z);
	btVector3 btEnd = btVector3(End.x, End.y, End.z);
	btCollisionWorld::ClosestRayResultCallback RayCallback(btStart, btEnd);
	physics->dynamicsWorld->rayTest(btStart, btEnd, RayCallback);
	if (RayCallback.hasHit()) {
		btVector3 btNewEnd = RayCallback.m_hitPointWorld;
		NewEnd = Vector3(btNewEnd.getX(), btNewEnd.getY(), btNewEnd.getZ());
		return RayCallback.m_collisionObject;
	}
	return nullptr;
}

void PhysicsScene::UpdateGame(float dt) {
//  world->GetMainCamera()->UpdateCamera(dt);
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_V)) {
		world->SwitchToFPS();
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_C)) {
		world->SwitchToTPS();
	}
  world->UpdateWorld(dt);

  UpdateKeys();
  renderer->Update(dt);
  physics->Update(dt);
  
  //bestcube->GetPhysicsObject()->GetRigidbody()->applyImpulse(btVector3(-1, 10000, 10), btVector3(0, -10, 0));
  //bestcube->GetPhysicsObject()->SetLinearVelocity(Vector3(100, 0, 0));
  //bestcube->GetPhysicsObject()->SetAngularVelocity(Vector3(0, 10, 0));
  //bestcube->GetPhysicsObject()->ApplyForce(Vector3(100000, 0, 10), Vector3(0, -10, 0));
  //bestcube->GetPhysicsObject()->ApplyTorque(Vector3(0, 10000000, 0));

  Vector3 newend;
  const btCollisionObject* test1 = Raycast(world->FindGameObjectWithTag(LayerAndTag::Tags::Player)->GetTransform().GetWorldPosition(), world->FindGameObjectWithTag(LayerAndTag::Tags::Resources)->GetTransform().GetWorldPosition(), newend);
  Debug::DrawLine(world->FindGameObjectWithTag(LayerAndTag::Tags::Player)->GetTransform().GetWorldPosition(), newend, Vector4(1, 0, 0, 1));

  const btCollisionObject* test2 = RaycastPosDir(world->FindGameObjectWithTag(LayerAndTag::Tags::Player)->GetTransform().GetWorldPosition(), Vector3(0,1,0), 200.0f, newend);
  Debug::DrawLine(world->FindGameObjectWithTag(LayerAndTag::Tags::Player)->GetTransform().GetWorldPosition(), newend, Vector4(1, 1, 0, 1));

  Debug::FlushRenderables();
  debugMenu.Update(dt, renderer);
  console.Update();
  hud.Update(dt, renderer);

  renderer->Render();
}

void PhysicsScene::DebugScene(float dt) {
}
