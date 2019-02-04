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
  physics->SetGravity(Vector3(0, -4, 0));
  physics->UseGravity(false);
  world->ShuffleConstraints(true);
  world->ShuffleObjects(true);

  Window::GetWindow()->ShowOSPointer(false);
  Window::GetWindow()->LockMouseToWindow(true);

  ResetWorld();
}

void ExampleScene::ResetWorld() {
  world->ClearAndErase();
  physics->Clear();

  // Floor
  AddFloorToWorld(Vector3(200, 0, 200));
}

ExampleScene::~ExampleScene() {
}

void ExampleScene::UpdateGame(float dt) {
  world->GetMainCamera()->UpdateCamera(dt);
  world->UpdateWorld(dt);

  renderer->Update(dt);
  physics->Update(dt);

  Debug::FlushRenderables();

  calculateNewFPS++;
  if (calculateNewFPS > 10) {
	  frameTime = dt;
	  calculateNewFPS = 0;
  }
  DebugScene(frameTime);

  renderer->Render();
}

void ExampleScene::DebugScene(float dt) {
	Debug::AddStringToDebugMenu("Graphics: ");
	Debug::AddStringToDebugMenu("FPS: " + std::to_string((int)(1 / dt)) + "." + std::to_string((int)(10 / dt) % 10));
	Debug::AddStringToDebugMenu("Verts: ");
	Debug::AddStringToDebugMenu("Draw Calls: ");
	Debug::AddStringToDebugMenu("Shadow Casters: ");
}