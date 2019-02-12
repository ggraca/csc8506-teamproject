#include "ExampleScene.h"
#include "../GameTechCommon/GameWorld.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/TextureLoader.h"

#include "../GameTechCommon/PositionConstraint.h"
#include "../Common/Assets.h"
#include <fstream>

using namespace NCL;
using namespace CSC8503;

ExampleScene::ExampleScene() : Scene() {
  Window::GetWindow()->ShowOSPointer(false);
  Window::GetWindow()->LockMouseToWindow(true);

  ResetWorld();
  debugMenu = DebugMenu();
  console = Console();
}

void ExampleScene::ResetWorld() {
  world->ClearAndErase();

  AddCubeToWorld(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0); //TODO Do these need to be deleted in destructor?!?!?!
  AddCubeToWorld(Vector3(0, 100, 0), Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), 45), Vector3(2, 2, 2), 1);
  AddCubeToWorld(Vector3(3.5, 130, 1), Quaternion::AxisAngleToQuaterion(Vector3(1, 1, 1), 45), Vector3(20, 20, 20), 0.05);
  AddSphereToWorld(Vector3(40, 100, 20), 1, 1);
  AddSphereToWorld(Vector3(41, 130, 20), 1, 1);
  InitMixedGridWorld(Vector3(47, 230, 20), 10, 10, 20, 20);
  // AddCylinderToWorld(Vector3(47, 230, 20), Vector3(2, 2, 2), 1);
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

  renderer->Render();
}

void ExampleScene::DebugScene(float dt) {

}