#include "NetworkExampleScene.h"
#include "PlayerPrefab.h"
#include "ResourcePrefab.h"
#include "WellPrefab.h"


NetworkExampleScene::NetworkExampleScene(bool& qG) : GameScene(qG) {
	ResetWorld();
}

void NetworkExampleScene::ResetWorld() {
	auto floor = new CubePrefab(Vec3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 0),
		Vec3(700, 10, 1000), 0, 1.0f, 1.0f);
	Mat4 floorTexMat = floor->GetComponent<RenderObject*>()->GetMaterial()->GetTextureMatrix();
	floor->GetComponent<RenderObject*>()->GetMaterial()->SetTextureMatrix(floorTexMat * Mat4::Scale(Vec3(32.0f, 32.0f, 32.0f)));
	floor->SetTag(LayerAndTag::Tags::Ground);

	//Player
	auto player = new PlayerPrefab(Vec3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 0), Vec3(10, 10, 10), 100, 0.2f, 0.4f);
	audio->SetPlayer(player);

	int size = 3;
	for (int i = -size; i <= size; i++) {
		for (int j = -size; j <= size; j++) {
			world->LateInstantiate(new ResourcePrefab(Vec3(i * 30, 190, j * 30), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 0), Vec3(5, 5, 5), 1000, 0.2f, 0.4f));
		}
	}

	//auto well = new WellPrefab(Vec3(50, 50, 50), Vec3(-100, 20, -100), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 90));

	world->LateInstantiateRecursively(player);
	world->LateInstantiate(floor);

	world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);
}