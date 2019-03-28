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
	console = Console();
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
		console.Toggle();
		DebugMenu::Toggle();
	}
	DebugMenu::Update(dt, renderer);
	console.Update();
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

	//EXAMPLE MATERIAL LOADING
	Material* cartMaterial = Assets::AssetManager::LoadMaterial("Cart Material", pbrShader);
	//Use GetMaterial in the prefab class!!! Name must match    ^^^^^^^^^^^^^^^
	//                                                          |||||||||||||||
	//                                                          |||||||||||||||
	//These textures aren't actually real - just an example
	/*TextureBase* pbrCartDiff = Assets::AssetManager::LoadTexture("Cart/cart_basecolor.png");
	TextureBase* pbrCartBump = Assets::AssetManager::LoadTexture("Cart/cart_normal.png");
	TextureBase* pbrCartRough = Assets::AssetManager::LoadTexture("Cart/cart_roughness.png");
	TextureBase* pbrCartMet = Assets::AssetManager::LoadTexture("Cart/cart_metalness.jpg");
	TextureBase* pbrCartAO = Assets::AssetManager::LoadTexture("Cart/cart_ao.png");
	cartMaterial->AddTextureParameter("diffuseTex", pbrCartDiff);
	cartMaterial->AddTextureParameter("bumpTex", pbrCartBump);
	cartMaterial->AddTextureParameter("roughnessTex", pbrCartRough);
	cartMaterial->AddTextureParameter("metalnessTex", pbrCartMet);
	cartMaterial->AddTextureParameter("aoTex", pbrCartAO);*/
	//If you don't have all of the textures just call this - it will fill in all the blank textures
	cartMaterial->InitBasicTextureParams();

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