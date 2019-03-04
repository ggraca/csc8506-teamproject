#include "PhysicsScene.h"
#include "GameWorld.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"

#include "../Common/Assets.h"
#include <fstream>
#include "Animator.h"

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
bool UpdateSome(GameObject *data)
{
	return Window::GetKeyboard()->KeyPressed(NCL::KEYBOARD_1);
}

bool ChangeSome(GameObject *data)//Just to test out
{
	return Window::GetKeyboard()->KeyPressed(NCL::KEYBOARD_2);
}

bool ChangeMore(GameObject *data)//Just to test out
{
	return Window::GetKeyboard()->KeyPressed(NCL::KEYBOARD_3);
}

void PhysicsScene::ResetWorld() {
  world->ClearAndErase();

  auto floor = AddCubeToWorld(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0, 1.0f, 1.0f); //TODO Do these need to be deleted in destructor?!?!?!
  floor->AddComponent<Animator*>((Component*)new Animator(floor));

  //Animation stuff as an example
  auto anim1 = new Animation(60);
  auto anim2 = new Animation(60);
  auto anim3 = new Animation(60);

  KeyFrame *frame1 = new KeyFrame();
  frame1->time = 10;
  frame1->localPosition = Vector3(200, -10, 200);
  frame1->localRotation =(Vector3(0, 0, 0));
  frame1->localScale = Vector3(700, 10, 1000);
  anim1->AddKeyFrame(frame1);
  auto animState1 = new AnimationState(anim1);

  KeyFrame *frame2 = new KeyFrame();
  frame2->time = 10;
  frame2->localPosition = Vector3(200, -10, 200);
  frame2->localRotation = (Vector3(0, 0, 0));
  frame2->localScale = Vector3(10, 10, 10);
  anim2->AddKeyFrame(frame2);
  auto animState2 = new AnimationState(anim2);

  KeyFrame *frame3 = new KeyFrame();
  frame3->time = 10.0f;
  frame3->localPosition = Vector3(200, -200, 200);
  frame3->localRotation = Vector3(180, 135, 0);
  frame3->localScale = Vector3(700, 10, 1000);
  anim3->AddKeyFrame(frame3);
  auto animState3 = new AnimationState(anim3);


  auto transition1 = new Transition();
  transition1->destinationState = animState2;
  transition1->transitionFunction = UpdateSome;

  auto transition2 = new Transition();
  transition2->destinationState = animState1;
  transition2->transitionFunction = ChangeSome;

  auto transition3 = new Transition();
  transition3->destinationState = animState3;
  transition3->transitionFunction = ChangeMore;

  animState1->AddTransition(transition1);
  animState1->AddTransition(transition3);
  animState3->AddTransition(transition2);

  animState2->AddTransition(transition2);
  floor->GetComponent<Animator*>()->AddAnimationState(animState1);
  floor->GetComponent<Animator*>()->AddAnimationState(animState2);
  floor->GetComponent<Animator*>()->AddAnimationState(animState3);


   //Player
  auto player = AddCubeToWorld(Vector3(0,20, 0), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 10, 0.2f, 0.4f);
  player->AddComponent<Player*>((Component*)new Player(player));
  player->SetTag(LayerAndTag::Tags::Player);
  player->GetComponent<PhysicsObject*>()->GetRigidbody()->setActivationState(DISABLE_DEACTIVATION);
  player->GetComponent<RenderObject*>()->GetMaterial()->SetColour(Vector4(1, 0, 0, 1));
  world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);
  auto resource1 = AddCubeToWorld(Vector3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f);
  resource1->SetName("Resource 1");
  resource1->AddComponent<Resource*>((Component*)new Resource(resource1));

  auto resource2 = AddCubeToWorld(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f);
  resource2->SetName("Resource 2");
  resource2->AddComponent<Resource*>((Component*)new Resource(resource2));
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
}

void PhysicsScene::DebugScene(float dt) {
}
