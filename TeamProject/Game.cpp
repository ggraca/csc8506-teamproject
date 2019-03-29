#include "Game.h"
#include "Scene.h"
#include "NetworkExampleScene.h"
#include "MenuScene.h"
#include "ExampleScene.h"
#include "PhysicsScene.h"
#include "LevelScene.h"

#include "InputManager.h"
#include "../Common/Material.h"

#include "FunctionTimer.h"


Game::Game() {
	renderer = new GameTechRenderer();
	audio = new CAudioEngine();
	InitialiseAssets(); // Should this be done in renderer? Or at least part of it?

	currentScene = new MenuScene(this);
	currentScene->SetRenderer(renderer);
	renderer->SetGameWorld(currentScene->GetGameWorld());
	Debug::SetRenderer(renderer);

	InputManager::InitializeButtonRelations();
}

Game::~Game() {
	delete audio;
	delete renderer;
	delete currentScene;

	Assets::AssetManager::FlushAssets();
	InputManager::Dispose();
}

void Game::Update(float dt) {
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_TILDE)) {
		Console::Toggle();
		DebugMenu::Toggle();
	}
	DebugMenu::Update(dt, renderer);
	Console::Update();
	if (network) network->Update();
	currentScene->Update(dt);
	renderer->Render();
	audio->Update();
	Debug::FlushRenderables();
}

void Game::ChangeCurrentScene(Scene* newScene, GameTechRenderer* r, bool server)
{ 
	currentScene = newScene;
	renderer->HUDState(true);
	currentScene->SetRenderer(renderer);
	renderer->SetGameWorld(currentScene->GetGameWorld());
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

	Material* cartMaterial = Assets::AssetManager::LoadMaterial("Cart Material", pbrShader);
	TextureBase* pbrCartDiff = Assets::AssetManager::LoadTexture("tex_extras_d.jpg");
	cartMaterial->AddTextureParameter("diffuseTex", pbrCartDiff);
	cartMaterial->InitBasicTextureParams();

	Material* castleMaterial = Assets::AssetManager::LoadMaterial("Castle Material", pbrShader);
	TextureBase* pbrCastleDiff = Assets::AssetManager::LoadTexture("BauWall_001.jpg");
	castleMaterial->AddTextureParameter("diffuseTex", pbrCastleDiff);
	castleMaterial->InitBasicTextureParams();

	Material* marketMaterial = Assets::AssetManager::LoadMaterial("Market Material", pbrShader);
	TextureBase* pbrMarketDiff = Assets::AssetManager::LoadTexture("TextureAtlas.png");
	TextureBase* pbrMarketBump = Assets::AssetManager::LoadTexture("Normals.png");
	TextureBase* pbrMarketRough = Assets::AssetManager::LoadTexture("that.png");
	marketMaterial->AddTextureParameter("diffuseTex", pbrMarketDiff);
	marketMaterial->AddTextureParameter("bumpTex", pbrMarketBump);
	marketMaterial->AddTextureParameter("roughnessTex", pbrMarketRough);
	marketMaterial->InitBasicTextureParams();

	Material* stallMaterial = Assets::AssetManager::LoadMaterial("Stall Material", pbrShader);
	TextureBase* pbrStallDiff = Assets::AssetManager::LoadTexture("defuse.tga");
	TextureBase* pbrStallBump = Assets::AssetManager::LoadTexture("defuse_DISP.tga");
	stallMaterial->AddTextureParameter("diffuseTex", pbrStallDiff);
	stallMaterial->AddTextureParameter("bumpTex", pbrStallBump);
	stallMaterial->InitBasicTextureParams();

	Material* tentMaterial = Assets::AssetManager::LoadMaterial("Tent Material", pbrShader);
	TextureBase* pbrTentDiff = Assets::AssetManager::LoadTexture("Market_Texture.jpg");
	tentMaterial->AddTextureParameter("diffuseTex", pbrTentDiff);
	tentMaterial->InitBasicTextureParams();

	Material* wallMaterial = Assets::AssetManager::LoadMaterial("Wall Material", pbrShader);
	TextureBase* pbrWallDiff = Assets::AssetManager::LoadTexture("texture.jpg");
	wallMaterial->AddTextureParameter("diffuseTex", pbrWallDiff);
	wallMaterial->InitBasicTextureParams();

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