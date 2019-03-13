#include "ExampleScene.h"
#include "PlayerPrefab.h"
#include "ResourcePrefab.h"
#include "Destructible.h"


ExampleScene::ExampleScene() : GameScene() {
	ResetWorld();
}

void ExampleScene::ResetWorld() {
	auto floor = new CubePrefab(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0),
		Vector3(700, 10, 1000), 0, 1.0f, 1.0f);
	Matrix4 floorTexMat = floor->GetComponent<RenderObject*>()->GetMaterial()->GetTextureMatrix();
	floor->GetComponent<RenderObject*>()->GetMaterial()->SetTextureMatrix(floorTexMat * Matrix4::Scale(Vector3(32.0f, 32.0f, 32.0f)));
	 //Player
	auto player = new PlayerPrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);
	audio->SetPlayer(player);

	auto resource1 = new ResourcePrefab(Vector3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource1->SetName("Resource 1");

	auto resource2 = new ResourcePrefab(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource2->SetName("Resource 2");

	auto des = new CubePrefab(Vector3(500, -10, 500), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(200, 200, 200), 0, 1.0f, 1.0f);
	des->AddComponent<Destructible*>(new Destructible(des));
	des->AddComponent<HealthManager*>(new HealthManager(des));
	des->GetComponent<HealthManager*>()->SetHealth(0);
	world->Instantiate(des);
	world->Instantiate(player);
	world->Instantiate(resource1);
	world->Instantiate(resource2);
	world->Instantiate(floor);

	world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);
  
	// OBJ file example
	//OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("Lamborghini_Aventador.obj");

	//// We need to pass world because father/son relationship is only possible among gameObjects in the world
	//// We might want to change this to allow any gameobject to have a vector of children
	//GameObject* go = GameObject::FromOBJ(objGeometry);
	//go->GetTransform().SetWorldPosition(Vector3(0, 5, 0));
	//go->GetTransform().SetLocalScale(Vector3(1, 1, 1));
	// world->AddGameObject(go); // TODO: We can uncomment this once we fix the bug mentioned above
}