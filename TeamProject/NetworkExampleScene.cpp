#include "NetworkExampleScene.h"
#include "PlayerPrefab.h"
#include "ResourcePrefab.h"
#include "WellPrefab.h"
#include "ConePrefab.h"
#include "WallPrefab.h"
#include "StallPrefab.h"
#include "TentPrefab.h"
#include "MarketPrefab.h"
#include "CastlePrefab.h"
#include "TowerPrefab.h"
#include "CannonPrefab.h"
#include "DWallPrefab.h"
#include "CartPrefab.h"
#include "Destructible.h"
#include "SpherePrefab.h"
#include "CylinderPrefab.h"

NetworkExampleScene::NetworkExampleScene(Game* g, bool& qG) : GameScene(g, qG) {
	ResetWorld();
}

void NetworkExampleScene::ResetWorld() {
	auto floor = new CubePrefab(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0),
		Vector3(700, 10, 1000), 0, 1.0f, 1.0f);
	floor->GetComponent<RenderObject*>()->SetBoundingRadius(1500.0f);

	Matrix4 floorTexMat = floor->GetComponent<RenderObject*>()->GetMaterial()->GetTextureMatrix();
	floor->GetComponent<RenderObject*>()->GetMaterial()->SetTextureMatrix(floorTexMat * Matrix4::Scale(Vector3(32.0f, 32.0f, 32.0f)));
	floor->SetTag(LayerAndTag::Tags::Ground);

	//Player
	auto player = new PlayerPrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);
	game->GetAudio()->SetPlayer(player);

	int size = 3;
	for (int i = -size; i <= size; i++) {
		for (int j = -size; j <= size; j++) {
			world->LateInstantiate(new ResourcePrefab(Vector3(i * 30, 190, j * 30), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f));
		}
	}

	world->LateInstantiateRecursively(player);
	world->LateInstantiate(floor);
}