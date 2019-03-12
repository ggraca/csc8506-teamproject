#include "Scene.h"
#include "../Common/Assets.h"
#include "GameWorld.h"
#include "CubePrefab.h"
#include "SpherePrefab.h"
#include "AudioEngine.h"
#include "InputManager.h"
#include "Light.h"


using namespace NCL;
using namespace CSC8503;

Scene::Scene() {
  world = new GameWorld();
  GameObject::SetGameWorld(world);
  renderer = new GameTechRenderer(*world);

  physics = new BulletPhysics(*world);
  physics->SetGravity(Vector3(-4.0f, -2000.81f, 0.0f));
  world->SetPhysics(physics);
  InputManager::InitializeButtonRelations();
  GameObject::SetGameWorld(world);
  audio = new CAudioEngine();
  world->SetAudio(audio);

  Debug::SetRenderer(renderer);
  InitialiseAssets();
}

void Scene::InitialiseAssets() {
  cubeMesh = (OGLMesh*) Assets::AssetManager::LoadMesh("Cube.msh");
  
  sphereMesh = (OGLMesh*) Assets::AssetManager::LoadMesh("sphere2.msh");
  renderer->SetLightMesh(sphereMesh);

  cylinderMesh = (OGLMesh*) Assets::AssetManager::LoadMesh("cylinder.obj");
  coneMesh = (OGLMesh*) Assets::AssetManager::LoadMesh("cone.obj");

  basicTex = Assets::AssetManager::LoadTexture("checkerboard.png");
  brickTex = Assets::AssetManager::LoadTexture("brick.png");
  woodTex = Assets::AssetManager::LoadTexture("wood1.jpg");
  dogsTex = Assets::AssetManager::LoadTexture("dogs.jpg");
  grassTex = Assets::AssetManager::LoadTexture("grass.jpg");
  ballTex = Assets::AssetManager::LoadTexture("smileyface.png");
  dogTex = Assets::AssetManager::LoadTexture("doge.png");

  pbrWoodDiff = Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_COLOR.jpg");
  pbrWoodBump = Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_NORM.jpg");
  pbrWoodSpec = Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_SPEC.jpg");
  pbrWoodMet = Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_SPEC.jpg");

  pbrShader = Assets::AssetManager::LoadShader("PBRShader", "pbrvert.glsl", "pbrfrag.glsl");

  basicMaterial = Assets::AssetManager::LoadMaterial("Basic Material", pbrShader);
  basicMaterial->AddTextureParameter("diffuseTex", pbrWoodDiff);
  basicMaterial->AddTextureParameter("bumpTex", pbrWoodBump);
  basicMaterial->AddTextureParameter("specularTex", pbrWoodSpec);
  basicMaterial->AddTextureParameter("metalnessTex", pbrWoodMet);

  floorMat = Assets::AssetManager::LoadMaterial("Floor Material", pbrShader);
  floorMat->AddTextureParameter("diffuseTex", pbrWoodDiff);
  floorMat->AddTextureParameter("bumpTex", pbrWoodBump);
  floorMat->AddTextureParameter("specularTex", pbrWoodSpec);
  floorMat->AddTextureParameter("metalnessTex", pbrWoodMet);
  Matrix4 texMatrix;
  texMatrix.ToIdentity();
  floorMat->SetTextureMatrix(texMatrix * Matrix4::Scale(Vector3(32.0f, 32.0f, 32.0f)));

  vector<std::string> faces
  {
	  "hw_alps/alps_ft.png",
	  "hw_alps/alps_bk.png",
	  "hw_alps/alps_up.png",
	  "hw_alps/alps_dn.png",
	  "hw_alps/alps_rt.png",
	  "hw_alps/alps_lf.png"
  };

  cubeMap = (OGLTexture*)TextureLoader::LoadAPICubeTexture(faces);
  renderer->skybox = cubeMap;

  InitWorld();
}

Scene::~Scene() {
  delete physics;
  delete renderer;
  delete world;

  Assets::AssetManager::FlushAssets();
  InputManager::Dispose();
}

void Scene::InitWorld() {
  world->ClearAndErase();

  GameObject* light = new GameObject("Directional Light");
  light->GetTransform().SetLocalOrientation(Quaternion::EulerAnglesToQuaternion(90.0f, 0.0f, 0.0f));
  light->AddComponent<Light*>(new Light(LightType::Directional, Vector4(1.0f, 1.0f, 1.0f, 1.0f), 2000.0f, 1.0f));
  world->Instantiate(light);
}

