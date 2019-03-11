#include <fstream>
#include <string>

#include "ExampleScene.h"
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



#include "../Common/OBJGeometry.h"

using namespace NCL;
using namespace CSC8503;

ExampleScene::ExampleScene() : Scene() {
  physics->SetGravity(Vector3(0, -100, 0));

  Window::GetWindow()->ShowOSPointer(false);
  Window::GetWindow()->LockMouseToWindow(true);

  ResetWorld();
  debugMenu = DebugMenu();
  console = Console();
  RegisterConsoleCommands();
}




void ExampleScene::ResetWorld() {

	auto floor = new CubePrefab(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0),
		Vector3(700, 10, 1000), 0, 1.0f, 1.0f);
	Matrix4 floorTexMat = floor->GetComponent<RenderObject*>()->GetMaterial()->GetTextureMatrix();
	floor->GetComponent<RenderObject*>()->GetMaterial()->SetTextureMatrix(floorTexMat * Matrix4::Scale(Vector3(32.0f, 32.0f, 32.0f)));
	 //Player
	auto player = new PlayerPrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);

	auto resource1 = new ResourcePrefab(Vector3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource1->SetName("Resource 1");

	auto resource2 = new ResourcePrefab(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource2->SetName("Resource 2");

	world->AddGameObject(player);
	world->AddGameObject(resource1);
	world->AddGameObject(resource2);
	world->AddGameObject(floor);

	world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);
  

  // OBJ file example
  //OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("full_shop.obj");

  LoadWorld();
  // We need to pass world because father/son relationship is only possible among gameObjects in the world
  // We might want to change this to allow any gameobject to have a vector of children
  //GameObject* go = GameObject::FromOBJ(objGeometry);
  //go->GetTransform().SetWorldPosition(Vector3(0, 30, 0));
  //go->GetTransform().SetLocalScale(Vector3(5, 5, 5));
  // world->AddGameObject(go); // TODO: We can uncomment this once we fix the bug mentioned above

  //OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("vws.obj");
  //GameObject* go = GameObject::FromOBJ(objGeometry);
  //go->GetTransform().SetWorldPosition(Vector3(0, 30, 0));
  //go->GetTransform().SetLocalScale(Vector3(0.5, 0.5, 0.5));


  


}

ExampleScene::~ExampleScene() {
}

void ExampleScene::UpdateGame(float dt) {

	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_V)) {
		world->SwitchToFPS();
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_C)) {
		world->SwitchToTPS();
	}

  world->UpdateWorld(dt);
  physics->Update(dt);
  renderer->Update(dt);
  world->ClearObjectsToDestroy();

  Debug::FlushRenderables();
  debugMenu.Update(dt, renderer);
  console.Update();
  hud.Update(dt, renderer);


  //Might want moved into a seperate function that handles input
  if (Window::GetKeyboard()->KeyPressed(KEYBOARD_TILDE)) {
	  console.Toggle();
	  debugMenu.Toggle();
  }


  /*if (Window::GetKeyboard()->KeyPressed(KEYBOARD_P)) {
	  delete hammer;
	  hammer = (OGLTexture*)Assets::AssetManager::LoadTexture("hammer.png");
	  renderer->hammer = hammer->GetObjectID();
	  delete gun;
	  gun = (OGLTexture*)Assets::AssetManager::LoadTexture("gun.png");
	  renderer->gun = gun->GetObjectID();
	  delete bomb;
	  bomb = (OGLTexture*)Assets::AssetManager::LoadTexture("bomb.png");
	  renderer->bomb = bomb->GetObjectID();
  }*/

  renderer->Render();

}

void CommandSetCameraPosition(vector<string> commandParams, void* data) {
	float x = stof(commandParams[1]);
	float y = stof(commandParams[2]);
	float z = stof(commandParams[3]);

	GameWorld* world = (GameWorld*)data;
	world->GetMainCamera()->GetTransform().SetWorldPosition(Vector3(x, y, z));
}

void ExampleScene::RegisterConsoleCommands() {
	console.RegisterCommand("setcamerapos", CommandSetCameraPosition, world);
}

void ExampleScene::LoadWorld() {
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

			if (a == 't') {
				OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("KofarGari.obj");
				GameObject* go = GameObject::FromOBJ(objGeometry);
				go->GetTransform().SetLocalScale(Vector3(1, 5, 1.31));
				go->GetTransform().SetWorldPosition(Vector3(size*i, 0, size*j));
				go->GetTransform().SetLocalOrientation(Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 90));			
			}
			if (a == 's') {
				OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("KofarGari.obj");
				GameObject* go = GameObject::FromOBJ(objGeometry);
				go->GetTransform().SetLocalScale(Vector3(1, 5, 1.31));
				go->GetTransform().SetWorldPosition(Vector3(size*i, 0, size*j));
				go->GetTransform().SetLocalOrientation(Quaternion::AxisAngleToQuaternion(Vector3(0, 1, 0), 90));
			}

			if (a == 'c') {
				OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("vws.obj");
				GameObject* go = GameObject::FromOBJ(objGeometry);
				go->GetTransform().SetLocalScale(Vector3(0.7,0.7,0.7));
				go->GetTransform().SetWorldPosition(Vector3(size*i, 42, size*j));
				go->GetTransform().SetLocalOrientation(Quaternion::AxisAngleToQuaternion(Vector3(0, 1, 0), 90));
			}



		}
	}

	infile.close();
}