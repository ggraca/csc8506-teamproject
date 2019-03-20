#include "Game.h"
#include "ExampleScene.h"
#include "PhysicsScene.h"
#include "InputManager.h"
#include "../Common/Material.h"


Game::Game() {
	renderer = new GameTechRenderer();
	// Should this be done in renderer? Or at least part of it?
	InitialiseAssets();

	// currentScene = new ExampleScene();
	currentScene = new PhysicsScene();
	currentScene->SetRenderer(renderer);
	
	renderer->SetGameWorld(currentScene->GetGameWorld());
	Debug::SetRenderer(renderer);

	Window::GetWindow()->ShowOSPointer(false);
	Window::GetWindow()->LockMouseToWindow(true);
	InputManager::InitializeButtonRelations();
}

Game::~Game() {
	delete renderer;
	delete currentScene;

	Assets::AssetManager::FlushAssets();
	InputManager::Dispose();
}

void Game::Update(float dt) {
	currentScene->Update(dt);
	renderer->Render();
}

void Game::InitialiseAssets() {
	OGLMesh* cubeMesh = (OGLMesh*)Assets::AssetManager::LoadMesh("Cube.msh");
	OGLMesh* sphereMesh = (OGLMesh*)Assets::AssetManager::LoadMesh("sphere2.msh");
	OGLMesh* cylinderMesh = (OGLMesh*)Assets::AssetManager::LoadMesh("cylinder.obj");
	OGLMesh* coneMesh = (OGLMesh*)Assets::AssetManager::LoadMesh("cone.obj");

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

	renderer->skybox = cubeMap;
	renderer->SetLightMesh(sphereMesh);
}