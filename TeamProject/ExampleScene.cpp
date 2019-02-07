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
  GenerateWorld();
}

void ExampleScene::ResetWorld() {
  world->ClearAndErase();
  physics->Clear();

  // Floor
  AddFloorToWorld(Vector3(0, 0, 0));
}

void ExampleScene::GenerateWorld()
{
	//Root object
	//AddCubeToWorld(Vector3(0, 0, 0), Vector3(10, 10, 10),0);
	AddWallToWorld(Vector3(-500, 10, 0), Vector3(5, 100, 500));
	AddWallToWorld(Vector3(0, 10, 500), Vector3(500, 100, 5));
	AddWallToWorld(Vector3(500, 10, 0), Vector3(5, 100, 500));
	AddWallToWorld(Vector3(0, 10, -500), Vector3(500, 100, 5));

	

	//Create root node 
	//Create children c
	//rootNode->AddChild(c)
	//Create another child b
	//c->AddChild(b)
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