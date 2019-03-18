#include <fstream>
#include <string>

#include "LevelScene.h"
#include "GameWorld.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/TextureLoader.h"
#include "../Common/Assets.h"


#include <fstream>
#include <string>
#include "PlayerPrefab.h"
#include "ResourcePrefab.h"
#include "WallPrefab.h"
#include "StallPrefab.h"
#include "TentPrefab.h"
#include "MarketPrefab.h"
#include "WellPrefab.h"
#include "CastlePrefab.h"
#include "TowerPrefab.h"
#include "CannonPrefab.h"
#include "DWallPrefab.h"
#include "CartPrefab.h"
#include "Destructible.h"


#include "../Common/OBJGeometry.h"

using namespace NCL;
using namespace CSC8503;

LevelScene::LevelScene() : Scene() {
	ResetWorld();
}

void LevelScene::ResetWorld() {

	auto floor = new CubePrefab(Vector3(2900, -10, 2900), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0),
		Vector3(3000, 10, 3000), 0, 1.0f, 1.0f);
	Matrix4 floorTexMat = floor->GetComponent<RenderObject*>()->GetMaterial()->GetTextureMatrix();
	floor->GetComponent<RenderObject*>()->GetMaterial()->SetTextureMatrix(floorTexMat * Matrix4::Scale(Vector3(32.0f, 32.0f, 32.0f)));
	//Player
	auto player = new PlayerPrefab(Vector3(3200, 260, 2500), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);

	audio->SetPlayer(player);
	audio->SetCamera(world->GetMainCamera());

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

	LoadWorld();
	
}

LevelScene::~LevelScene() {
}


void LevelScene::LoadWorld() {
	std::ifstream infile(Assets::DATADIR + "level1.txt");
	float size, x_length, z_length;
	infile >> size;
	infile >> x_length;
	infile >> z_length;

	if (!infile) { std::cout << "no file" << std::endl; }


	for (float i = 0; i < (x_length);i++) {
		for (float j = 0; j < (z_length); j++) {
			char a;
			infile >> a;

			if (a == 'w') {
				auto TopWall = new WallPrefab(Vector3(37.0f, 40.0f, 37.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
			}
			if (a == 's') {
				auto SideWall = new WallPrefab(Vector3(37.0f, 40.0f, 37.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 0.0f, 0.0f), 90.0f));
			}

			if (a == 'c') {
				auto Stall = new StallPrefab(Vector3(0.7f, 0.7f, 0.7f), Vector3(size*i, 42.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
			}
					
			if (a == 'C') {
				auto Stall = new StallPrefab(Vector3(0.7f, 0.7f, 0.7f), Vector3(size*i, 42.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), -90.0f));
			}

			if (a == 'v') {
				auto Stall = new StallPrefab(Vector3(0.7f, 0.7f, 0.7f), Vector3(size*i, 42.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 0.0f));
			}

			if (a == 'V') {
				auto Stall = new StallPrefab(Vector3(0.7f, 0.7f, 0.7f), Vector3(size*i, 42.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 180.0f));
			}

			if (a == 't') {
				auto Tent = new TentPrefab(Vector3(10.0f, 10.0f, 10.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
			}

			if (a == 'm') {
				auto Market = new MarketPrefab(Vector3(40.0f, 40.0f, 40.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
			}

			if (a == 'M') {
				auto Market = new MarketPrefab(Vector3(40.0f, 40.0f, 40.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), -90.0f));
			}

			if (a == 'n') {
				auto Market = new MarketPrefab(Vector3(40.0f, 40.0f, 40.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 0.0f));
			}

			if (a == 'N') {
				auto Market = new MarketPrefab(Vector3(40.0f, 40.0f, 40.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 180.0f));
			}

			if (a == 'W') {
				auto Well = new WellPrefab(Vector3(50.0f, 50.0f, 50.0f), Vector3(size*i, 20.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
			}

			if (a == 'k') {
				auto Cart = new CartPrefab(Vector3(3.0f, 3.0f, 3.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
			}

			if (a == 'b') {
				auto Castle = new CastlePrefab(Vector3(0.03f, 0.03f, 0.03f), Vector3(size*i, -30.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
			}

			if (a == '1') {
				auto Tower = new TowerPrefab(Vector3(50.0f, 100.0f, 50.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
				auto Cannon = new CannonPrefab(Vector3(1.0f, 1.0f, 1.0f), Vector3(size*i, 300.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(1.0f, 0.0f, 0.0f), -90.0f));
			}

			if (a == '2') {
				auto DWall = new DWallPrefab(Vector3(1.4f, 1.0f, 1.4f), Vector3(size*i, 0.0f, size*j+37.5f), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), -90.0f));
			}

			if (a == '3') {
				auto DWall = new DWallPrefab(Vector3(1.4f, 1.0f, 1.4f), Vector3(size*i, 0.0f, size*j + 93.75f), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
			}

			if (a == '4') {
				auto DWall = new DWallPrefab(Vector3(1.4f, 1.0f, 1.4f), Vector3(size*i + 37.5f, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 0.0f));
			}

			if (a == '5') {
				auto DWall = new DWallPrefab(Vector3(1.4f, 1.0f, 1.4f), Vector3(size*i + 93.75f, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 180.0f));
			}

			if (a == '6') {
				auto DWall = new DWallPrefab(Vector3(1.6f, 1.0f, 1.6f), Vector3(size*i - 37.5f, 0.0f, size*j - 37.5f), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), -45.0f));
			}

			if (a == '7') {
				auto DWall = new DWallPrefab(Vector3(1.6f, 1.0f, 1.6f), Vector3(size*i + 37.5f, 0.0f, size*j + 37.5f), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), -225.0f));
			}

			if (a == '8') {
				auto DWall = new DWallPrefab(Vector3(1.6f, 1.0f, 1.6f), Vector3(size*i + 37.5f, 0.0f, size*j - 37.5f), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 225.0f));
			}

			if (a == '9') {
				auto DWall = new DWallPrefab(Vector3(1.6f, 1.0f, 1.6f), Vector3(size*i - 37.5f, 0.0f, size*j + 37.5f), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 45.0f));
			}
		}
	}

	infile.close();
}