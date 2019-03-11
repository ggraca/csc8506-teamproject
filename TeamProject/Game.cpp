#include "Game.h"
#include "ExampleScene.h"

Game::Game() {
	renderer = new GameTechRenderer();
	currentScene = new ExampleScene();
	
	renderer->SetGameWorld(currentScene->GetGameWorld());

	OGLMesh* sphereMesh = (OGLMesh*)Assets::AssetManager::LoadMesh("sphere2.msh");
	renderer->SetLightMesh(sphereMesh);
}

Game::~Game() {
	delete renderer;
	delete currentScene;
}

void Game::Update(float dt) {
	currentScene->UpdateGame(dt);
	renderer->Render();
}
