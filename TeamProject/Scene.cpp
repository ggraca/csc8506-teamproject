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

void CommandSetCameraPosition(vector<string> commandParams, void* data) {
	float x = stof(commandParams[1]);
	float y = stof(commandParams[2]);
	float z = stof(commandParams[3]);

	GameWorld* world = (GameWorld*)data;
	world->GetMainCamera()->GetTransform().SetWorldPosition(Vector3(x, y, z));
}

void Scene::RegisterConsoleCommands() {
	console.RegisterCommand("setcamerapos", CommandSetCameraPosition, world);
}
