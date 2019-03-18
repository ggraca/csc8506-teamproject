#include "Scene.h"


Scene::Scene() {
	world = new GameWorld();
	GameObject::SetGameWorld(world);

	physics = new BulletPhysics(*world);
	physics->SetGravity(Vector3(0, -100, 0));
	world->SetPhysics(physics);

	audio = new CAudioEngine();
	audio->SetCamera(world->GetMainCamera());
	world->SetAudio(audio);
}

Scene::~Scene() {
	delete audio;
	delete physics;
	delete world;
}

void Scene::Update(float dt) {
	EarlyUpdate(dt);

	world->UpdateWorld(dt);
	physics->Update(dt);
	audio->Update();

	LateUpdate(dt);
}