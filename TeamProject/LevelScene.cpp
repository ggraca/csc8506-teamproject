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

LevelScene::LevelScene(bool& quitGame) : GameScene(quitGame) {
	ResetWorld();
}

void LevelScene::ResetWorld() {

	auto floor = new CubePrefab(Vec3(2900, -10, 2900), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 0),
		Vec3(3000, 10, 3000), 0, 1.0f, 1.0f);
	Mat4 floorTexMat = floor->GetComponent<RenderObject*>()->GetMaterial()->GetTextureMatrix();
	floor->GetComponent<RenderObject*>()->GetMaterial()->SetTextureMatrix(floorTexMat * Mat4::Scale(Vec3(32.0f, 32.0f, 32.0f)));

	//Player
	auto player = new PlayerPrefab(Vec3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 0), Vec3(10, 10, 10), 10, 0.2f, 0.4f);

	//This 1
	audio->SetPlayer(player);
	audio->SetCamera(world->GetMainCamera());

	auto resource1 = new ResourcePrefab(Vec3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 0), Vec3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource1->SetName("Resource 1");

	auto resource2 = new ResourcePrefab(Vec3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 0), Vec3(5, 5, 5), 10, 0.2f, 0.4f);
	resource2->SetName("Resource 2");

	
	world->Instantiate(resource1);
	world->Instantiate(resource2);
	world->Instantiate(floor);
	world->InstantiateRecursively(player);

	//This 2
	//world->Instantiate(player);

	//This 3
	world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);

	LoadWorld();
	std::map<std::string, OBJGeometry*>* objs = Assets::AssetManager::GetOBJMeshes();
	
}

LevelScene::~LevelScene() {
}

void LevelScene::LoadWorld() {
	std::ifstream infile(Assets::DATADIR + "level1.txt");
	int size, x_length, z_length;
	infile >> size;
	infile >> x_length;
	infile >> z_length;

	if (!infile) { std::cout << "no file" << std::endl; }


	for (unsigned i = 0; i < (x_length);i++) {
		for (unsigned j = 0; j < (z_length); j++) {
			char a;
			infile >> a;

			if (a == 'w') {
				auto TopWall = new WallPrefab(Vec3(37, 40, 37), Vec3(size*i, 0, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 90));
			}
			if (a == 's') {
				auto SideWall = new WallPrefab(Vec3(37, 40, 37), Vec3(size*i, 0, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 0, 0), 90));
			}

			if (a == 'c') {
				auto Stall = new StallPrefab(Vec3(0.7, 0.7, 0.7), Vec3(size*i, 42, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 90));
			}
					
			if (a == 'C') {
				auto Stall = new StallPrefab(Vec3(0.7, 0.7, 0.7), Vec3(size*i, 42, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), -90));
			}

			if (a == 'v') {
				auto Stall = new StallPrefab(Vec3(0.7, 0.7, 0.7), Vec3(size*i, 42, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 0));
			}

			if (a == 'V') {
				auto Stall = new StallPrefab(Vec3(0.7, 0.7, 0.7), Vec3(size*i, 42, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 180));
			}

			if (a == 't') {
				auto Tent = new TentPrefab(Vec3(10, 10, 10), Vec3(size*i, 0, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 90));
			}

			if (a == 'm') {
				auto Market = new MarketPrefab(Vec3(40, 40, 40), Vec3(size*i, 0, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 90));
			}

			if (a == 'M') {
				auto Market = new MarketPrefab(Vec3(40, 40, 40), Vec3(size*i, 0, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), -90));
			}

			if (a == 'n') {
				auto Market = new MarketPrefab(Vec3(40, 40, 40), Vec3(size*i, 0, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 0));
			}

			if (a == 'N') {
				auto Market = new MarketPrefab(Vec3(40, 40, 40), Vec3(size*i, 0, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 180));
			}

			if (a == 'W') {
				auto Well = new WellPrefab(Vec3(1, 1, 1), Vec3(size*i, 0, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 90));
			}

			if (a == 'k') {
				auto Cart = new CartPrefab(Vec3(3, 3, 3), Vec3(size*i, 0, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 90));
			}

			if (a == 'b') {
				auto Castle = new CastlePrefab(Vec3(0.03, 0.03, 0.03), Vec3(size*i, -30, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 90));
			}

			if (a == '1') {
				auto Tower = new TowerPrefab(Vec3(50, 100, 50), Vec3(size*i, 0, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 90));
				auto Cannon = new CannonPrefab(Vec3(2.0, 2.0, 2.0), Vec3(size*i, 300, size*j), Quaternion::AxisAngleToQuaternion(Vec3(1, 0, 0), 0));
			}

			if (a == '2') {
				auto DWall = new DWallPrefab(Vec3(1.4, 1, 1.4), Vec3(size*i, 0, size*j+37.5), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), -90));
			}

			if (a == '3') {
				auto DWall = new DWallPrefab(Vec3(1.4, 1, 1.4), Vec3(size*i, 0, size*j + 93.75), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 90));
			}

			if (a == '4') {
				auto DWall = new DWallPrefab(Vec3(1.4, 1, 1.4), Vec3(size*i + 37.5, 0, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 0));
			}

			if (a == '5') {
				auto DWall = new DWallPrefab(Vec3(1.4, 1, 1.4), Vec3(size*i + 93.75, 0, size*j), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 180));
			}

			if (a == '6') {
				auto DWall = new DWallPrefab(Vec3(1.6, 1, 1.6), Vec3(size*i - 37.5, 0, size*j - 37.5), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), -45));
			}

			if (a == '7') {
				auto DWall = new DWallPrefab(Vec3(1.6, 1, 1.6), Vec3(size*i + 37.5, 0, size*j + 37.5), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), -225));
			}

			if (a == '8') {
				auto DWall = new DWallPrefab(Vec3(1.6, 1, 1.6), Vec3(size*i + 37.5, 0, size*j - 37.5), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 225));
			}

			if (a == '9') {
				auto DWall = new DWallPrefab(Vec3(1.6, 1, 1.6), Vec3(size*i - 37.5, 0, size*j + 37.5), Quaternion::AxisAngleToQuaternion(Vec3(0, 1, 0), 45));
			}
		}
	}

	infile.close();
}