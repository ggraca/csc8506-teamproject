#include <fstream>
#include <string>

#include "ExampleScene.h"
#include "GameWorld.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/TextureLoader.h"
#include "../Common/Assets.h"
#include "../Common/OBJGeometry.h"

using namespace NCL;
using namespace CSC8503;

ExampleScene::ExampleScene() : Scene() {
  physics->SetGravity(Vector3(0, -4, 0));

  Window::GetWindow()->ShowOSPointer(false);
  Window::GetWindow()->LockMouseToWindow(true);

  ResetWorld();
  debugMenu = DebugMenu();
  console = Console();
  RegisterConsoleCommands();
}

void ExampleScene::ResetWorld() {
  // Floor
  AddCubeToWorld(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0,0.2f);

  //Player
  auto player = AddCubeToWorld(Vector3(0, 20, 0), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100);
  player->AddComponent<Player*>((ScriptObject*)new Player(player));
  player->SetTag(LayerAndTag::Tags::Player);
  world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);

  auto resource1 = AddCubeToWorld(Vector3(50, 20, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 100, 0.2f);
  auto resource2 = AddCubeToWorld(Vector3(100, 20, 100), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 100, 0.2f);
  resource1->SetName("Resource 1");
  resource2->SetName("Resource 2");
  resource1->AddComponent<Resource*>((ScriptObject*)new Resource(resource1));
  resource2->AddComponent<Resource*>((ScriptObject*)new Resource(resource2));

  // OBJ file example
  OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("cone.obj");

  // We need to pass world because father/son relationship is only possible among gameObjects in the world
  // We might want to change this to allow any gameobject to have a vector of children
  GameObject* go = GameObject::FromOBJ(objGeometry);
  go->GetTransform().SetWorldPosition(Vector3(0, 5, 0));
  go->GetTransform().SetLocalScale(Vector3(1, 1, 1));
  // world->AddGameObject(go); // TODO: We can uncomment this once we fix the bug mentioned above

  /*int size = objGeometry->GetChildren().size();
  MeshGeometry* mesh = objGeometry->GetChildren()[size - 1];*/
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