#include "PhysicsScene.h"
#include "PlayerPrefab.h"
#include "ResourcePrefab.h"
#include "HammerControl.h"


PhysicsScene::PhysicsScene() : GameScene() {
	ResetWorld();
}

void PhysicsScene::ResetWorld() {
	auto floor = new CubePrefab(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0, 1.0f, 1.0f);

	InitPlayer();

	auto resource1 = new ResourcePrefab(Vector3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f,0.4f);
	resource1->SetName("Resource 1");

	auto resource2 = new ResourcePrefab(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource2->SetName("Resource 2");
  
	auto des = new CubePrefab(Vector3(500, -10, 500), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(200, 200, 200), 0, 1.0f, 1.0f);
	des->AddComponent<Destructible*>(new Destructible(des));
	des->AddComponent<HealthManager*>(new HealthManager(des));
	des->GetComponent<HealthManager*>()->SetHealth(8);

	world->Instantiate(des);
	world->Instantiate(resource1);
	world->Instantiate(resource2);
	world->Instantiate(floor);
}

PhysicsScene::~PhysicsScene() {
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
	//if (Window::GetKeyboard()->KeyPressed(KEYBOARD_U)) {
	//	renderer->WeaponState(2, true); //Hammer
	//	renderer->WeaponState(3, true); //Gun
	//	renderer->WeaponState(4, true); //Bomb
	//}
	//if (Window::GetKeyboard()->KeyPressed(KEYBOARD_I)) {
	//	renderer->WeaponState(2, false); //Hammer
	//	renderer->WeaponState(3, false); //Gun
	//	renderer->WeaponState(4, false); //Bomb
	//}
	//if (Window::GetKeyboard()->KeyPressed(KEYBOARD_Y))
	//{
	//	hud.hp -= 5;
	//	renderer->health -= 0.05f;
	//}
	//if (Window::GetKeyboard()->KeyPressed(KEYBOARD_T))
	//{
	//	hud.hp = 100;
	//	renderer->health = 1.0f;
	//}
	////HUD TESTING ENDS
	//if (Window::GetKeyboard()->KeyPressed(KEYBOARD_TILDE)) {
	//	console.Toggle();
	//	debugMenu.Toggle();
	//}
}

void PhysicsScene::InitPlayer()
{
	auto player = new PlayerPrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 10, 0.2f, 0.4f);
	auto * playerLeft = new CubePrefab(CubePrefab::PrefabType::GUN);
	auto * playerRight = new CubePrefab(CubePrefab::PrefabType::GUN);
	auto * handle = new CubePrefab(CubePrefab::PrefabType::HANDLE);

	playerLeft->SetParent(player);
	playerRight->SetParent(player);
	handle->SetParent(player);
	Transform * hammerHead = new Transform();
	hammerHead->SetParent(&handle->GetTransform());
	hammerHead->SetLocalPosition(Vector3(0, 1, 0));

	playerRight->GetTransform().SetLocalPosition(Vector3(2, 0, 1));
	playerLeft->GetTransform().SetLocalPosition(Vector3(-2, 0, 1));
	handle->GetTransform().SetLocalPosition(Vector3(-1, 0, 2));
	player->GetComponent<GunControl*>()->SetRightGun(playerRight);
	player->GetComponent<GunControl*>()->SetLeftGun(playerLeft);
	player->GetComponent<HammerControl*>()->SetHandle(handle);

	world->Instantiate(player);
	world->Instantiate(playerLeft);
	world->Instantiate(playerRight);
	world->Instantiate(handle);
	world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);
	audio->SetPlayer(player);
}

void PhysicsScene::LateUpdate(float dt) {
	GameScene::LateUpdate(dt);
	UpdateKeys();
  
	//bestcube->GetPhysicsObject()->GetRigidbody()->applyImpulse(btVector3(-1, 10000, 10), btVector3(0, -10, 0));
	//bestcube->GetPhysicsObject()->SetLinearVelocity(Vector3(100, 0, 0));
	//bestcube->GetPhysicsObject()->SetAngularVelocity(Vector3(0, 10, 0));
	//bestcube->GetPhysicsObject()->ApplyForce(Vector3(100000, 0, 10), Vector3(0, -10, 0));
	//bestcube->GetPhysicsObject()->ApplyTorque(Vector3(0, 10000000, 0));
}