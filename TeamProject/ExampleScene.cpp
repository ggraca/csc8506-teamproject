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



ExampleScene::ExampleScene() : Scene() {
  physics->SetGravity(Vector3(0, -4, 0));
  inputManager = new InputManager();
  Window::GetWindow()->ShowOSPointer(false);
  Window::GetWindow()->LockMouseToWindow(true);

  ResetWorld();
  debugMenu = DebugMenu();
  console = Console();
  RegisterConsoleCommands();
  GameObject::SetGameWorld(world);


}




void ExampleScene::ResetWorld() {
  world->ClearAndErase();

  // Floor
  AddCubeToWorld(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0);

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

InputManager * ExampleScene::GetInputManager() const
{
	return inputManager;
}


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
