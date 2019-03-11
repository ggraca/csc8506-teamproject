#include "PhysicsScene.h"
#include "GameWorld.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "Light.h"



#include "../Common/Assets.h"
#include <fstream>
#include "PlayerPrefab.h"
#include "ResourcePrefab.h"



using namespace NCL;
using namespace CSC8503;

PhysicsScene::PhysicsScene() : Scene() {
  Window::GetWindow()->ShowOSPointer(false);
  Window::GetWindow()->LockMouseToWindow(true);

  ResetWorld();
  debugMenu = DebugMenu();
  console = Console();
  InitStateMachine();
}

void PhysicsScene::ResetWorld() {
 
  auto floor = new CubePrefab(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0, 1.0f, 1.0f);
   //Player
  auto player = new PlayerPrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);
  
  auto resource1 = new ResourcePrefab(Vector3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f,0.4f);
  resource1->SetName("Resource 1");

  auto resource2 = new ResourcePrefab(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
  resource2->SetName("Resource 2");

  audio->SetPlayer(player);
  audio->SetCamera(world->GetMainCamera());
  
  world->AddGameObject(player);
  world->AddGameObject(resource1);
  world->AddGameObject(resource2);
  world->AddGameObject(floor);


  world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);
}

PhysicsScene::~PhysicsScene() {
	delete tempTex;
	delete worldState;
	delete objectStateMachine;
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

	//Menu
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_B)) {
		showMenu = !showMenu;		
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
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_TILDE)) {
		console.Toggle();
		debugMenu.Toggle();
	}
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
  menu.Update(dt, renderer);
  objectStateMachine->Update();
  physics->Update(dt);
  renderer->Update(dt);
  world->ClearObjectsToDestroy();
  
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

void PhysicsScene::ShowMenu()
{
	float offset = 200.0f;
	for (MenuEntry me : menu.MenuEntries())
	{
		Vector4 colour;
		if (me.selected) colour = Vector4(1, 0, 0, 1);
		else colour = Vector4(0, 0, 1, 1);
		renderer->DrawString(me.menuTitle, Vector2(50, Window::GetWindow()->GetScreenSize().y / 2 + offset), colour);
		offset -= 50.0f;
	}
	//renderer->DrawString("MAIN MENU", 
	//renderer->DrawString("PRESS 1 TO PLAY LEVEL ONE", Vector2(50, Window::GetWindow()->GetScreenSize().y / 2 + 150), Vector4(0, 0, 1, 1));
	//renderer->DrawString("PRESS 2 TO PLAY LEVEL TWO", Vector2(50, Window::GetWindow()->GetScreenSize().y / 2 + 100), Vector4(0, 0, 1, 1));
	//renderer->DrawString("PRESS 3 TO PLAY LEVEL THREE", Vector2(50, Window::GetWindow()->GetScreenSize().y / 2 + 50), Vector4(0, 0, 1, 1));
	//renderer->DrawString("PRESS 4 TO PLAY LEVEL FOUR", Vector2(50, Window::GetWindow()->GetScreenSize().y / 2), Vector4(0, 0, 1, 1));
	//renderer->DrawString("PRESS R TO RESET LEVEL", Vector2(50, Window::GetWindow()->GetScreenSize().y / 2 - 50), Vector4(0, 0, 1, 1));
	//renderer->DrawString("PRESS Q TO TOGGLE FREE ROAM CAMERA", Vector2(50, Window::GetWindow()->GetScreenSize().y / 2 - 100), Vector4(0, 0, 1, 1));
}

void PhysicsScene::UsedForMenu(void* data)
{
	//This doesn't do anything but helps with the menu toggle.
}

void PhysicsScene::InitStateMachine()
{
	objectStateMachine = new StateMachine();

	//World
	std::function<void(PhysicsScene*, void*)> WorldFunc = &PhysicsScene::UsedForMenu;
	worldState = new LevelState(WorldFunc, (void *)& worldState, this);
	objectStateMachine->AddState(worldState);

	//Menu
	std::function<void(PhysicsScene*)> menuFunc1 = &PhysicsScene::ShowMenu;
	MenuState* mainMenu = new MenuState(menuFunc1, this);
	objectStateMachine->AddState(mainMenu);

	//Menu transition
	ShowMenuTransition<bool&, bool>* worldToMainMenu = new ShowMenuTransition <bool&, bool >(ShowMenuTransition <bool&, bool>::EqualsTransition, showMenu, true, worldState, mainMenu, this);
	ShowMenuTransition<bool&, bool>* mainMenuToWorld = new ShowMenuTransition <bool&, bool>(ShowMenuTransition <bool&, bool>::EqualsTransition, showMenu, false, mainMenu, worldState, this);
	objectStateMachine->AddTransition(worldToMainMenu);
	objectStateMachine->AddTransition(mainMenuToWorld);
	
}

void PhysicsScene::DebugScene(float dt) {
}
