#include "ExampleScene.h"
#include "../GameTechCommon/GameWorld.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/TextureLoader.h"

#include "../GameTechCommon/PositionConstraint.h"
#include "../Common/Assets.h"

#include <fstream>


#include "btBulletDynamicsCommon.h"



using namespace NCL;
using namespace CSC8503;


ExampleScene::ExampleScene() : Scene(-9.81) {
 /* physics->SetGravity(Vector3(0, -9.81, 0));
  physics->UseGravity(true);*/
  /*world->ShuffleConstraints(true);
  world->ShuffleObjects(true);*/

  Window::GetWindow()->ShowOSPointer(false);
  Window::GetWindow()->LockMouseToWindow(true);

  ResetWorld();
  debugMenu = DebugMenu();
  console = Console();
}

ExampleScene::ExampleScene(float g) : Scene(g) {
	Window::GetWindow()->ShowOSPointer(false);
	Window::GetWindow()->LockMouseToWindow(true);

	ResetWorld();
	debugMenu = DebugMenu();
	console = Console();
}

void ExampleScene::ResetWorld() {
  world->ClearAndErase();
 // physics->Clear();

  //AddFloorToWorld(Vector3(200, -10, 200));
  AddCubeToWorld(Vector3(200, -10, 200), Vector3(500, 10, 500), 0); //TODO Do these need to be deleted in destructor?!?!?!
  AddCubeToWorld(Vector3(0, 100, 0), Vector3(20, 20, 20), 1);
  AddSphereToWorld(Vector3(40, 5, 20), 10, 1);

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