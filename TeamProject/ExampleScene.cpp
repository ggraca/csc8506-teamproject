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
  AddCubeToWorld(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0,0.2f);
  //Player
  auto player = AddCubeToWorld(Vector3(0, 20, 0), Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100);
  player->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
  player->AddScript((ScriptObject*)new Player(player));
  player->SetTag(LayerAndTag::Tags::Player);

  auto resource1 = AddCubeToWorld(Vector3(50, 20, 50), Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 100, 0.2f);
  auto resource2 = AddCubeToWorld(Vector3(100, 20, 100), Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 100, 0.2f);
  resource1->SetName("Resource 1");
  resource2->SetName("Resource 2");
  resource1->AddScript((ScriptObject*)new Resource(resource1));
  resource2->AddScript((ScriptObject*)new Resource(resource2));

}

ExampleScene::~ExampleScene() {
	delete inputManager;
}

void ExampleScene::UpdateGame(float dt) {
  world->GetMainCamera()->UpdateCamera(dt);
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

  renderer->Render();
  
}

//InputManager * ExampleScene::GetInputManager() const
//{
//	return inputManager;
//}


void CommandSetCameraPosition(vector<string> commandParams, void* data) {
	float x = stof(commandParams[1]);
	float y = stof(commandParams[2]);
	float z = stof(commandParams[3]);

	GameWorld* world = (GameWorld*)data;
	world->GetMainCamera()->SetPosition(Vector3(x, y, z));
}

void ExampleScene::RegisterConsoleCommands() {
	console.RegisterCommand("setcamerapos", CommandSetCameraPosition, world);
}
