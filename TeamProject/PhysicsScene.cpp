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

  audio->LoadBank(Assets::SOUNDSDIR + "Test2\\Build\\Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
  audio->LoadBank(Assets::SOUNDSDIR + "Test2\\Build\\Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
 /* audio->LoadEvent("event:/Music");
  audio->LoadEvent("event:/Jump");
  audio->LoadEvent("event:/Land");*/

  audio->PlayEvent("event:/Music", Vector3(0, 0, 0));

  //audio->  getEvent("event:/UI/Cancel", &cancelDescription);
  //cancelDescription->createInstance(&cancelInstance);

  //cancelInstance->start();
  //cancelInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);


 // int x = audio->PlaySounds(Assets::SOUNDSDIR + "1.mp3");

  auto floor = AddCubeToWorld(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0, 1.0f, 1.0f); //TODO Do these need to be deleted in destructor?!?!?!
  floor->SetName("Floor");
  floor->SetTag(LayerAndTag::Tags::Occupied);

   //Player
  auto player = AddCubeToWorld(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);
  player->AddComponent<Player*>((Component*)new Player(player));
  player->SetTag(LayerAndTag::Tags::Player);
  player->GetComponent<PhysicsObject*>()->GetRigidbody()->setActivationState(DISABLE_DEACTIVATION);
  player->GetComponent<RenderObject*>()->GetMaterial()->SetColour(Vector4(1, 0, 0, 1));
  world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);

  audio->SetPlayer(player);
  audio->SetCamera(world->GetMainCamera());

  auto resource1 = AddCubeToWorld(Vector3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f);
  resource1->SetName("Resource 1");
  resource1->SetTag(LayerAndTag::Tags::Resources);
  resource1->AddComponent<Resource*>((Component*)new Resource(resource1));

  auto resource2 = AddCubeToWorld(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f);
  resource2->SetName("Resource 2");
  resource2->SetTag(LayerAndTag::Tags::Destructable);
  resource2->AddComponent<Resource*>((Component*)new Resource(resource2));

  AddSphereToWorld(Vector3(40, 10, 20), 5, 1);
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

  Debug::FlushRenderables();
  debugMenu.Update(dt, renderer);
  console.Update();
  hud.Update(dt, renderer);

  renderer->Render();

  audio->Update();
}

void PhysicsScene::DebugScene(float dt) {
}
