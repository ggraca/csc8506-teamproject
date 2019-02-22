#include "ExampleScene.h"
#include "GameWorld.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/TextureLoader.h"

#include "../Common/Assets.h"

#include <fstream>
#include <string>




using namespace NCL;
using namespace CSC8503;

InputManager * ExampleScene::inputManager = nullptr;

ExampleScene::ExampleScene() : Scene() {
  physics->SetGravity(Vector3(0, -4, 0));
  
  Window::GetWindow()->ShowOSPointer(false);
  Window::GetWindow()->LockMouseToWindow(true);

  ResetWorld();
  debugMenu = DebugMenu();
  console = Console();
  RegisterConsoleCommands();

  if(!inputManager){ inputManager = new InputManager(); }//Static guy initializations
  GameObject::SetGameWorld(world);


}




void ExampleScene::ResetWorld() {
  world->ClearAndErase();

  // Floor
  AddCubeToWorld(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0,0.2f);
  //Player
  auto player = AddCubeToWorld(Vector3(0, 20, 0), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100);
  player->AddScript((ScriptObject*)new Player(player));
  player->SetTag(LayerAndTag::Tags::Player);
  world->GetMainCamera()->GetScript<CameraControl*>()->SetPlayer(player);

  auto resource1 = AddCubeToWorld(Vector3(50, 20, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 100, 0.2f);
  auto resource2 = AddCubeToWorld(Vector3(100, 20, 100), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 100, 0.2f);
  resource1->SetName("Resource 1");
  resource2->SetName("Resource 2");
  resource1->AddScript((ScriptObject*)new Resource(resource1));
  resource2->AddScript((ScriptObject*)new Resource(resource2));

}

ExampleScene::~ExampleScene() {
	delete inputManager;
}

void ExampleScene::UpdateGame(float dt) {

	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_V)) {
		world->SwitchToFPS();
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_C)) {
		world->SwitchToTPS();
	}

  world->UpdateWorld(dt);

  renderer->Update(dt);
  physics->Update(dt);

  Debug::FlushRenderables();
  debugMenu.Update(dt, renderer);
  console.Update();
  hud.Update(dt, renderer);


  //Might want moved into a seperate function that handles input
  if (Window::GetKeyboard()->KeyPressed(KEYBOARD_TILDE)) {
	  console.Toggle();
	  debugMenu.Toggle();
  }

  if (Window::GetKeyboard()->KeyPressed(KEYBOARD_K)) {
	  if (hud.hp > 4)
	  {
		hud.hp -= 5;
		renderer->health -= 0.05;
		renderer->UpdateHealthQuad();
	  }
  }
  if (Window::GetKeyboard()->KeyPressed(KEYBOARD_L)) {
	  if (hud.hp >= 0)
	  {
		  hud.hp = 100;
		  renderer->health = 1;
		  renderer->UpdateHealthQuad();
	  }
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
