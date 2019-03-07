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
	world->ClearAndErase();

	AddCubeToWorld(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0, 1.0f, 1.0f); //TODO Do these need to be deleted in destructor?!?!?!

	 //Player
	auto player = new PlayerPrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);
	player->AddComponent<RenderObject*>(new RenderObject(&player->GetTransform(), cubeMesh, basicMaterial));
	player->GetComponent<RenderObject*>()->GetMaterial()->SetColour(Vector4(1, 0, 0, 1));
	player->GetComponent<RenderObject*>()->SetMaterialInstanced();

	auto resource1 = new ResourcePrefab(Vector3(50, 190, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource1->SetName("Resource 1");
	resource1->AddComponent<RenderObject*>(new RenderObject(&resource1->GetTransform(), cubeMesh, basicMaterial));
	resource1->GetComponent<RenderObject*>()->SetMaterialInstanced();
	resource1->GetComponent<Resource*>()->Reset();

	auto resource2 = new ResourcePrefab(Vector3(50, 130, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	resource2->SetName("Resource 2");
	resource2->AddComponent<RenderObject*>(new RenderObject(&resource2->GetTransform(), cubeMesh, basicMaterial));
	resource2->GetComponent<RenderObject*>()->SetMaterialInstanced();
	resource2->GetComponent<Resource*>()->Reset();

	world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);
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
	  hammer = (OGLTexture*)TextureLoader::LoadAPITexture("hammer.png");
	  renderer->hammer = hammer->GetObjectID();
	  delete gun;
	  gun = (OGLTexture*)TextureLoader::LoadAPITexture("gun.png");
	  renderer->gun = gun->GetObjectID();
	  delete bomb;
	  bomb = (OGLTexture*)TextureLoader::LoadAPITexture("bomb.png");
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
