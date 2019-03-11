#include "Scene.h"
#include "Light.h"


Scene::Scene() {
	world = new GameWorld();
	GameObject::SetGameWorld(world);

	physics = new BulletPhysics(*world);
	physics->SetGravity(Vector3(0, -100, 0));
	world->SetPhysics(physics);

	audio = new CAudioEngine();
	world->SetAudio(audio);

	InitWorld();
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
	// audio->Update();

	LateUpdate(dt);
}

void Scene::InitWorld() {
	world->ClearAndErase();

	GameObject* light = new GameObject("Directional Light");
	light->GetTransform().SetLocalOrientation(Quaternion::EulerAnglesToQuaternion(90.0f, 0.0f, 0.0f));
	light->AddComponent<Light*>(new Light(LightType::Directional, Vector4(1.0f, 1.0f, 1.0f, 1.0f), 2000.0f, 1.0f));
	world->Instantiate(light);
}

