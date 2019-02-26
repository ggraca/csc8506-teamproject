#include <fstream>
#include <string>

#include "NetworkScene.h"
#include "GameWorld.h"
#include "../Common/TextureLoader.h"
#include "../Common/Assets.h"

using namespace NCL;

NetworkScene::NetworkScene() : Scene() {
	ResetWorld();

	GameObject::SetGameWorld(world);
}

void NetworkScene::InitNetwork() {

}

void NetworkScene::ResetWorld() {
	world->ClearAndErase();

	// Floor
	AddCubeToWorld(Vector3(200, -10, 200), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(700, 10, 1000), 0, 0.2f);
}

void NetworkScene::UpdateGame(float dt) {
	world->UpdateWorld(dt);

	renderer->Update(dt);
	physics->Update(dt);
	renderer->Render();
}