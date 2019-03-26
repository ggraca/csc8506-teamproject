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
#include "CannonScript.h"

using namespace NCL;
using namespace CSC8503;

LevelScene::LevelScene(bool& quitGame) : GameScene(quitGame) {
	ResetWorld();
}

void LevelScene::ResetWorld() {

	auto floor = new CubePrefab(Vector3(2900, -10, 2900), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0),
		Vector3(3000, 10, 3000), 0, 1.0f, 1.0f);
	Matrix4 floorTexMat = floor->GetComponent<RenderObject*>()->GetMaterial()->GetTextureMatrix();
	floor->GetComponent<RenderObject*>()->GetMaterial()->SetTextureMatrix(floorTexMat * Matrix4::Scale(Vector3(32.0f, 32.0f, 32.0f)));
	floor->SetTag(LayerAndTag::Tags::Ground);

	//Player
	auto player = new PlayerPrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 10, 0.2f, 0.4f);

	//This 1
	audio->SetPlayer(player);
	audio->SetCamera(world->GetMainCamera());
	

	auto resource1 = new ResourcePrefab(Vector3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource1->SetName("Resource 1");

	auto resource2 = new ResourcePrefab(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 10, 0.2f, 0.4f);
	resource2->SetName("Resource 2");

	auto resource3 = new ResourcePrefab(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource2->SetName("Resource 2");

	auto resource4 = new ResourcePrefab(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource2->SetName("Resource 2");

	auto resource5 = new ResourcePrefab(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource2->SetName("Resource 2");

	auto resource6 = new ResourcePrefab(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource2->SetName("Resource 2");

	
	world->LateInstantiate(resource1);
	world->LateInstantiate(resource2);
	world->LateInstantiate(resource3);
	world->LateInstantiate(resource4);
	world->LateInstantiate(resource5);
	world->LateInstantiate(resource6);
	world->LateInstantiate(floor);
	world->LateInstantiateRecursively(player);


	LoadWorld();
	std::map<std::string, OBJGeometry*>* objs = Assets::AssetManager::GetOBJMeshes();
	
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
			switch (a)
			{
				case 'w' :new WallPrefab(Vector3(37.0f, 40.0f, 37.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
					break;
				case 's' :new WallPrefab(Vector3(37.0f, 40.0f, 37.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 0.0f, 0.0f), 90.0f));
					break;
				case 'c' :new StallPrefab(Vector3(0.7f, 0.7f, 0.7f), Vector3(size*i, 42.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
					break;
				case 'C' :new StallPrefab(Vector3(0.7f, 0.7f, 0.7f), Vector3(size*i, 42.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), -90.0f));
					break;
				case 'v' :new StallPrefab(Vector3(0.7f, 0.7f, 0.7f), Vector3(size*i, 42.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 0.0f));
					break;
				case 'V' :new StallPrefab(Vector3(0.7f, 0.7f, 0.7f), Vector3(size*i, 42.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 180.0f));
					break;
				case 't' :new TentPrefab(Vector3(10.0f, 10.0f, 10.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
					break;
				case 'm' :new MarketPrefab(Vector3(40.0f, 40.0f, 40.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
					break;
				case 'M' :new MarketPrefab(Vector3(40.0f, 40.0f, 40.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), -90.0f));
					break;
				case 'n' :new MarketPrefab(Vector3(40.0f, 40.0f, 40.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 0.0f));
					break;
				case 'N' :new MarketPrefab(Vector3(40.0f, 40.0f, 40.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 180.0f));
					break;
				case 'W' :new WellPrefab(Vector3(2.0f, 2.0f, 2.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
					break;
				case 'k' :new CartPrefab(Vector3(3.0f, 3.0f, 3.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
					break;
				case 'b' :new CastlePrefab(Vector3(0.03f, 0.03f, 0.03f), Vector3(size*i, -30.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
					break;
				case '1' :new TowerPrefab(Vector3(50.0f, 100.0f, 50.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
						 //new CannonPrefab(Vector3(2.0f, 2.0f, 2.0f), Vector3(size*i, 300.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(1.0f, 0.0f, 0.0f), 0.0f));
					break;
				case '2' :new DWallPrefab(Vector3(1.4f, 1.0f, 1.4f), Vector3(size*i, 0.0f, size*j + 37.5f), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), -90.0f));
					break;
				case '3' :new DWallPrefab(Vector3(1.4f, 1.0f, 1.4f), Vector3(size*i, 0.0f, size*j + 93.75f), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
					break;
				case '4' :new DWallPrefab(Vector3(1.4f, 1.0f, 1.4f), Vector3(size*i + 37.5f, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 0.0f));
					break;
				case '5' :new DWallPrefab(Vector3(1.4f, 1.0f, 1.4f), Vector3(size*i + 93.75f, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 180.0f));
					break;
				case '6' :new DWallPrefab(Vector3(1.6f, 1.0f, 1.6f), Vector3(size*i - 37.5f, 0.0f, size*j - 37.5f), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), -45.0f));
					break;
				case '7' :new DWallPrefab(Vector3(1.6f, 1.0f, 1.6f), Vector3(size*i + 37.5f, 0.0f, size*j + 37.5f), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), -225.0f));
					break;
				case '8' :new DWallPrefab(Vector3(1.6f, 1.0f, 1.6f), Vector3(size*i + 37.5f, 0.0f, size*j - 37.5f), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 225.0f));
					break;
				case '9' :new DWallPrefab(Vector3(1.6f, 1.0f, 1.6f), Vector3(size*i - 37.5f, 0.0f, size*j + 37.5f), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 45.0f));
					break;
				case 'r':new TowerPrefab(Vector3(50.0f, 100.0f, 50.0f), Vector3(size*i, 0.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f));
					new CannonPrefab(Vector3(2.0f, 2.0f, 2.0f), Vector3(size*i, 300.0f, size*j), Quaternion::AxisAngleToQuaternion(Vector3(1.0f, 0.0f, 0.0f), 0.0f));
					break;
			}
		}
	}

	infile.close();
}