#include "Scene.h"


Scene::Scene(Game* g) : game(g){
	world = new GameWorld();
	GameObject::SetGameWorld(world);

	physics = new BulletPhysics(*world);
	physics->SetGravity(Vector3(0, -100, 0));
	world->SetPhysics(physics);

	game->GetAudio()->SetCamera(world->GetMainCamera());
	world->SetAudio(game->GetAudio());
}

Scene::~Scene() {
	delete physics;
	delete world;
}

void Scene::Update(float dt) {
	EarlyUpdate(dt);

	world->UpdateWorld(dt);
	physics->Update(dt);

	LateUpdate(dt);
}