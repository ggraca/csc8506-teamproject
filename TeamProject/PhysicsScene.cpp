#include "PhysicsScene.h"
#include "PlayerPrefab.h"
#include "ResourcePrefab.h"
#include "HammerControl.h"
#include "ParticleSystem.h"
#include "WallPrefab.h"
#include "DWallPrefab.h"
#include "CannonPrefab.h"
#include "CartPrefab.h"
#include "CastlePrefab.h"
#include "MarketPrefab.h"
#include "StallPrefab.h"
#include "TentPrefab.h"
#include "TowerPrefab.h"
#include "WellPrefab.h"

PhysicsScene::PhysicsScene(Game* g, bool& qG) : GameScene(g, qG) {
	ResetWorld();
}

void PhysicsScene::ResetWorld() {
	auto floor = new CubePrefab(Vector3(200, -40, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0, 1.0f, 1.0f);
	floor->GetComponent<RenderObject*>()->SetBoundingRadius(1500.0f);

	auto player = new PlayerPrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 10, 0.2f, 0.4f);

	world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);
	game->GetAudio()->SetPlayer(player);
	game->GetAudio()->SetCamera(world->GetMainCamera());

	auto resource1 = new ResourcePrefab(Vector3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 10, 0.2f,0.4f);
	resource1->SetName("Resource 1");

	auto resource2 = new ResourcePrefab(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 10, 0.2f, 0.4f);
	resource2->SetName("Resource 2");
  
	//auto des = new CubePrefab(Vector3(500, 100, 500), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(200, 200, 200), 0, 0.0f, 1.0f);
	//des->AddComponent<Destructible*>(new Destructible(des));
	//des->AddComponent<HealthManager*>(new HealthManager(des));
	//des->GetComponent<HealthManager*>()->SetHealth(8);
	//des->SetName("Destructible");

	//auto des2 = new CubePrefab(Vector3(0, 100, 0), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(200, 200, 200), 0, 0.0f, 1.0f);
	//des2->AddComponent<Destructible*>(new Destructible(des2));
	//des2->AddComponent<HealthManager*>(new HealthManager(des2));
	//des2->GetComponent<HealthManager*>()->SetHealth(8);
	//des2->SetName("Destructible");

//	auto wall = new WallPrefab(Vector3(37.0f, 40.0f, 37.0f), Vector3(0, 0, 0), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));

//	auto stall = new StallPrefab(Vector3(0.7f, 0.7f, 0.7f), Vector3(100, 42.0f, 0), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
		
//	auto tent = new TentPrefab(Vector3(10.0f, 10.0f, 10.0f), Vector3(200, 0.0f, 0), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
		
//	auto market = new MarketPrefab(Vector3(40.0f, 40.0f, 40.0f), Vector3(300, 0.0f, 0), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
	
//	auto well = new WellPrefab(Vector3(2.0f, 2.0f, 2.0f), Vector3(400, 0.0f, 0), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
	
//	auto cart = new CartPrefab(Vector3(3.0f, 3.0f, 3.0f), Vector3(500, 0.0f, 0), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
		
//	auto castle = new CastlePrefab(Vector3(0.03f, 0.03f, 0.03f), Vector3(600, -30.0f, 0), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
	
//	auto tower = new TowerPrefab(Vector3(50.0f, 100.0f, 50.0f), Vector3(700, 0.0f, 0), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));

//	auto cannon = new CannonPrefab(Vector3(2.0f, 2.0f, 2.0f), Vector3(800, 0.0f, 0), Quaternion::AxisAngleToQuaternion(Vector3(1.0f, 0.0f, 0.0f), 0.0f));
	
	auto dwall = new DWallPrefab(Vector3(1.6f, 1.0f, 1.6f), Vector3(900, 0.0f, 0), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 45.0f));
		

	/*world->Instantiate(des);
	world->Instantiate(des2);*/
	world->Instantiate(resource1);
	world->Instantiate(resource2);
	world->Instantiate(floor);
//	world->Instantiate(wall);
	world->InstantiateRecursively(player);

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

	////HUD TESTING BEGINS
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

void PhysicsScene::LateUpdate(float dt) {
	GameScene::LateUpdate(dt);
	UpdateKeys();
}
