#include "ExampleScene.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"

#include "../CSC8503Common/PositionConstraint.h"
#include "../../Common/Assets.h"

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
  renderer->Render();
}