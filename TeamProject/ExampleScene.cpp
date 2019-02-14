#include "ExampleScene.h"
#include "../GameTechCommon/GameWorld.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/TextureLoader.h"

#include "../GameTechCommon/PositionConstraint.h"
#include "../Common/Assets.h"

#include <fstream>
#include <string>




using namespace NCL;
using namespace CSC8503;


ExampleScene::ExampleScene() : Scene() {
  physics->SetGravity(Vector3(0, -4, 0));
  physics->UseGravity(false);
  world->ShuffleConstraints(true);
  world->ShuffleObjects(true);

  Window::GetWindow()->ShowOSPointer(false);
  Window::GetWindow()->LockMouseToWindow(true);

  ResetWorld();
  debugMenu = DebugMenu();
  console = Console();
  RegisterConsoleCommands();

  anim = new Animation(60);
  KeyFrame* f1 = new KeyFrame();
  f1->time = 5;
  f1->localPosition = Vector3(100, 100, 100);
  f1->localRotation = dummy->GetTransform().GetLocalOrientation().ToEuler();
  f1->localScale = dummy->GetTransform().GetLocalScale();
  anim->AddKeyFrame(f1);
  KeyFrame* f2 = new KeyFrame();
  f2->time = 10;
  f2->localPosition = Vector3(200, 0, 200);
  f2->localRotation = dummy->GetTransform().GetLocalOrientation().ToEuler();
  f2->localScale = Vector3(1,1,1);
  anim->AddKeyFrame(f2);
  KeyFrame* f3 = new KeyFrame();
  f3->time = 15;
  f3->localPosition = Vector3(200, 0, 200);
  f3->localRotation = dummy->GetTransform().GetLocalOrientation().ToEuler();
  f3->localScale = Vector3(10, 10, 10);
  anim->AddKeyFrame(f3);
  KeyFrame* f4 = new KeyFrame();
  f4->time = 17;
  f4->localPosition = Vector3(200, 0, 200);
  f4->localRotation = dummy->GetTransform().GetLocalOrientation().ToEuler();
  f4->localScale = Vector3(5, 5, 5);
  KeyFrame* f5 = new KeyFrame();
  f5->time = 25;
  f5->localPosition = Vector3(200, 0, 200);
  f5->localRotation = Vector3(0,180,360);
  f5->localScale = Vector3(5, 5, 5);
  anim->AddKeyFrame(f5);

  anim->Play();
}

void ExampleScene::ResetWorld() {
  world->ClearAndErase();
  physics->Clear();

  // Floor
  dummy = AddFloorToWorld(Vector3(200, 0, 200));
  
}

ExampleScene::~ExampleScene() {
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
  anim->UpdateAnimation(dummy,dt);

  //Might want moved into a seperate function that handles input
  if (Window::GetKeyboard()->KeyPressed(KEYBOARD_TILDE)) {
	  console.Toggle();
	  debugMenu.Toggle();
  }

  renderer->Render();
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