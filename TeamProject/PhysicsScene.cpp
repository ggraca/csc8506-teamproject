#include "PhysicsScene.h"
#include "PlayerPrefab.h"
#include "ResourcePrefab.h"
#include "HammerControl.h"
#include "ParticleSystem.h"


PhysicsScene::PhysicsScene(bool& qG) : GameScene(qG) {
	ResetWorld();
}

void PhysicsScene::ResetWorld() {
	auto floor = new CubePrefab(Vec3(200, -40, 200), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 0), Vec3(700, 10, 1000), 0, 1.0f, 1.0f);

	auto player = new PlayerPrefab(Vec3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 0), Vec3(10, 10, 10), 10, 0.2f, 0.4f);

	world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);
	audio->SetPlayer(player);
	audio->SetCamera(world->GetMainCamera());

	auto resource1 = new ResourcePrefab(Vec3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 0), Vec3(5, 5, 5), 10, 0.2f,0.4f);
	resource1->SetName("Resource 1");

	auto resource2 = new ResourcePrefab(Vec3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 0), Vec3(5, 5, 5), 10, 0.2f, 0.4f);
	resource2->SetName("Resource 2");
  
	auto des = new CubePrefab(Vec3(500, 100, 500), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 0), Vec3(200, 200, 200), 0, 0.0f, 1.0f);
	des->AddComponent<Destructible*>(new Destructible(des));
	des->AddComponent<HealthManager*>(new HealthManager(des));
	des->GetComponent<HealthManager*>()->SetHealth(8);
	des->SetName("Destructible");

	auto des2 = new CubePrefab(Vec3(0, 100, 0), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 0), Vec3(200, 200, 200), 0, 0.0f, 1.0f);
	des2->AddComponent<Destructible*>(new Destructible(des2));
	des2->AddComponent<HealthManager*>(new HealthManager(des2));
	des2->GetComponent<HealthManager*>()->SetHealth(8);
	des2->SetName("Destructible");

	world->Instantiate(des);
	world->Instantiate(des2);
	world->Instantiate(resource1);
	world->Instantiate(resource2);
	world->Instantiate(floor);
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
