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

NetworkExampleScene::NetworkExampleScene(bool& qG) : GameScene(qG) {
	ResetWorld();
}

void NetworkExampleScene::ResetWorld() {
	auto floor = new CubePrefab(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0),
		Vector3(700, 10, 1000), 0, 1.0f, 1.0f);
	Matrix4 floorTexMat = floor->GetComponent<RenderObject*>()->GetMaterial()->GetTextureMatrix();
	floor->GetComponent<RenderObject*>()->GetMaterial()->SetTextureMatrix(floorTexMat * Matrix4::Scale(Vector3(32.0f, 32.0f, 32.0f)));
	floor->SetTag(LayerAndTag::Tags::Ground);

	//Player
	auto player = new PlayerPrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);
	audio->SetPlayer(player);

	int size = 3;
	for (int i = -size; i <= size; i++) {
		for (int j = -size; j <= size; j++) {
			world->LateInstantiate(new ResourcePrefab(Vector3(i * 30, 190, j * 30), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f));
		}
	}

	auto DWall = new DWallPrefab(Vector3(2, 1, 1), Vector3(19 + 37.5, 0, 20 + 37.5), Quaternion::AxisAngleToQuaternion(Vector3(0, 1, 0), 180));
	DWall->SetTag(LayerAndTag::Tags::Ground);

	for (int i = -size; i <= size; i++) {
		for (int j = -size; j <= size; j++) {
			new SpherePrefab(Vector3(i * 100, 590, j * 30), 10, 10, 0.5, 0.5);
			world->LateInstantiate(new SpherePrefab(Vector3(i * 100, 590, j * 30), 10, 10, 0.5, 0.5));
		}
	}

	for (int i = -size; i <= size; i++) {
		for (int j = -size; j <= size; j++) {
			
			world->LateInstantiate(new ConePrefab(Vector3(50, 100, 50), Vector3(i * 100, 1630, j * 30), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), 10, 0.5, 0.5));
		}
	}

	for (int i = -size; i <= size; i++) {
		for (int j = -size; j <= size; j++) {	
			world->LateInstantiate(new CylinderPrefab(Vector3(50, 100, 50), Vector3(i * 100, 1590, j * 30), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), 10, 0.5, 0.5));
		}
	}

	auto ball = new SpherePrefab(Vector3(1.6, 30, 1.6), 10, 10, 0.5, 0.5);
	auto cone = new ConePrefab(Vector3(50, 100, 50), Vector3(300, 500, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), 10, 0.5, 0.5);
	auto cylinder = new CylinderPrefab(Vector3(50, 100, 50), Vector3(300, 300, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), 10, 0.5, 0.5);

	//auto well = new WellPrefab(Vector3(50, 50, 50), Vector3(-100, 20, -100), Quaternion::AxisAngleToQuaternion(Vector3(0, 1, 0), 90));

	world->LateInstantiateRecursively(player);
	world->LateInstantiate(floor);
	world->LateInstantiate(ball);
	world->LateInstantiate(cone);
	world->LateInstantiate(DWall);
	world->LateInstantiate(cylinder);

	world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);
}