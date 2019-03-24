#include "Game.h"
#include "NetworkExampleScene.h"
#include "MenuScene.h"
#include "ExampleScene.h"
#include "PhysicsScene.h"
#include "LevelScene.h"

#include "InputManager.h"
#include "../Common/Material.h"


Game::Game() {
	renderer = new GameTechRenderer();
	InitialiseAssets(); // Should this be done in renderer? Or at least part of it?

	currentScene = new MenuScene(this);
	currentScene->SetRenderer(renderer);
	renderer->SetGameWorld(currentScene->GetGameWorld());
	Debug::SetRenderer(renderer);
	
	//currentScene = new NetworkExampleScene();	
	//currentScene->SetRenderer(renderer);
	//currentScene->GetGameWorld()->SetNetwork(network->GetEntity());
	//network->GetEntity()->SetWorld(currentScene->GetGameWorld());

	InputManager::InitializeButtonRelations();
}

Game::~Game() {
	delete renderer;
	delete currentScene;

	Assets::AssetManager::FlushAssets();
	InputManager::Dispose();
}

void Game::Update(float dt) {
	if (network) network->Update();
	currentScene->Update(dt);
	renderer->Render();
}

void Game::ChangeCurrentScene(Scene* newScene, GameTechRenderer* r, bool server)
{ 
	currentScene = nullptr;
	delete currentScene;
	currentScene = newScene; 
	currentScene->SetRenderer(renderer);
	network = new NetworkManager(server);
	renderer->SetGameWorld(currentScene->GetGameWorld());
	currentScene->GetGameWorld()->SetNetwork(network->GetEntity());
	network->GetEntity()->SetWorld(currentScene->GetGameWorld());
	Debug::SetRenderer(renderer);
}

void Game::InitialiseAssets() {
	MeshGeometry* cubeMesh = Assets::AssetManager::LoadMesh("Cube.msh");
	MeshGeometry* sphereMesh = Assets::AssetManager::LoadMesh("sphere2.msh");
	MeshGeometry* cylinderMesh = Assets::AssetManager::LoadMesh("cylinder.obj");
	MeshGeometry* coneMesh = Assets::AssetManager::LoadMesh("cone.obj");

	TextureBase* basicTex = Assets::AssetManager::LoadTexture("checkerboard.png");
	TextureBase* brickTex = Assets::AssetManager::LoadTexture("brick.png");
	TextureBase* woodTex = Assets::AssetManager::LoadTexture("wood1.jpg");
	TextureBase* dogsTex = Assets::AssetManager::LoadTexture("dogs.jpg");
	TextureBase* grassTex = Assets::AssetManager::LoadTexture("grass.jpg");
	TextureBase* ballTex = Assets::AssetManager::LoadTexture("smileyface.png");
	TextureBase* dogTex = Assets::AssetManager::LoadTexture("doge.png");

	TextureBase* pbrWoodDiff = Assets::AssetManager::LoadTexture("Oak Floor/oakfloor_basecolor.png");
	TextureBase* pbrWoodBump = Assets::AssetManager::LoadTexture("Oak Floor/oakfloor_normal.png");
	TextureBase* pbrWoodRough = Assets::AssetManager::LoadTexture("Oak Floor/oakfloor_roughness.png");
	TextureBase* pbrWoodMet = Assets::AssetManager::LoadTexture("black.jpg");
	TextureBase* pbrWoodAO = Assets::AssetManager::LoadTexture("Oak Floor/oakfloor_roughness.png"); // Doesn't load ao tex??

	ShaderBase* pbrShader = Assets::AssetManager::LoadShader("PBRShader", "pbrvert.glsl", "pbrfrag.glsl");

	using NCL::Material;
	Material* basicMaterial = Assets::AssetManager::LoadMaterial("Basic Material", pbrShader);
	basicMaterial->AddTextureParameter("diffuseTex", pbrWoodDiff);
	basicMaterial->AddTextureParameter("bumpTex", pbrWoodBump);
	basicMaterial->AddTextureParameter("roughnessTex", pbrWoodRough);
	basicMaterial->AddTextureParameter("metalnessTex", pbrWoodMet);
	basicMaterial->AddTextureParameter("aoTex", pbrWoodAO);

	vector<std::string> faces {
		"hw_alps/alps_ft.png",
		"hw_alps/alps_bk.png",
		"hw_alps/alps_up.png",
		"hw_alps/alps_dn.png",
		"hw_alps/alps_rt.png",
		"hw_alps/alps_lf.png"
	};

	TextureBase* cubeMap = (OGLTexture*)TextureLoader::LoadAPICubeTexture(faces);

	renderer->SetSkyBox(cubeMap);
	renderer->SetLightMesh(sphereMesh);
}