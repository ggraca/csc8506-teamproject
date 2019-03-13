#include "PhysicsScene.h"
#include "PlayerPrefab.h"
#include "ResourcePrefab.h"
#include "ParticleSystem.h"


PhysicsScene::PhysicsScene() : GameScene() {
	ResetWorld();
}

void PhysicsScene::ResetWorld() {
	auto floor = new CubePrefab(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0, 1.0f, 1.0f);

	auto player = new PlayerPrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);
	//Remove particle system - it's a dummy to test
	player->AddComponent<ParticleSystem*>(new ParticleSystem(5.0f, 100, 100, 5.0f, 1.0f, Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.2f, 0.0f, 0.2f), 10.0f, Assets::AssetManager::LoadTexture("doge.png")));
	audio->SetPlayer(player);

	auto resource1 = new ResourcePrefab(Vector3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f,0.4f);
	resource1->SetName("Resource 1");

	auto resource2 = new ResourcePrefab(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource2->SetName("Resource 2");
  
	world->Instantiate(player);
	world->Instantiate(resource1);
	world->Instantiate(resource2);
	world->Instantiate(floor);
	world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);
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

void PhysicsScene::LateUpdate(float dt) {
	GameScene::LateUpdate(dt);
	UpdateKeys();
  
	//bestcube->GetPhysicsObject()->GetRigidbody()->applyImpulse(btVector3(-1, 10000, 10), btVector3(0, -10, 0));
	//bestcube->GetPhysicsObject()->SetLinearVelocity(Vector3(100, 0, 0));
	//bestcube->GetPhysicsObject()->SetAngularVelocity(Vector3(0, 10, 0));
	//bestcube->GetPhysicsObject()->ApplyForce(Vector3(100000, 0, 10), Vector3(0, -10, 0));
	//bestcube->GetPhysicsObject()->ApplyTorque(Vector3(0, 10000000, 0));
}