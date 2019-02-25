#include "Scene.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/Assets.h"
#include "GameWorld.h"
#include "InputManager.h"

using namespace NCL;
using namespace CSC8503;

Scene::Scene() {
  world = new GameWorld();
  renderer = new GameTechRenderer(*world);
  physics = new BulletPhysics(*world);
  physics->SetGravity(Vector3(-4, -60.81, 0));
  world->SetPhysics(physics);
  InputManager::InitializeButtonRelations();

  Debug::SetRenderer(renderer);

  InitialiseAssets();
}

void Scene::InitialiseAssets() {
  cubeMesh = new OGLMesh("Cube.msh");
  cubeMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
  cubeMesh->UploadToGPU();

  sphereMesh = new OGLMesh("sphere2.msh");
  sphereMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
  sphereMesh->UploadToGPU();
  renderer->SetLightMesh(sphereMesh);

  cylinderMesh = new OGLMesh("cylinder.obj");
  cylinderMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
  cylinderMesh->UploadToGPU();

  coneMesh = new OGLMesh("cone.obj");
  coneMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
  coneMesh->UploadToGPU();

  basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
  brickTex = (OGLTexture*)TextureLoader::LoadAPITexture("brick.png");
  woodTex = (OGLTexture*)TextureLoader::LoadAPITexture("wood1.jpg");
  dogsTex = (OGLTexture*)TextureLoader::LoadAPITexture("dogs.jpg");
  grassTex = (OGLTexture*)TextureLoader::LoadAPITexture("grass.jpg");

  ballTex = (OGLTexture*)TextureLoader::LoadAPITexture("smileyface.png");
  dogTex = (OGLTexture*)TextureLoader::LoadAPITexture("doge.png");

  pbrWoodDiff = (OGLTexture*)Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_COLOR.jpg");
  pbrWoodBump = (OGLTexture*)Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_NORM.jpg");
  pbrWoodSpec = (OGLTexture*)Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_DISP.jpg");
  pbrWoodMet = (OGLTexture*)Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_SPEC.jpg");

  pbrShader = (OGLShader*)Assets::AssetManager::LoadShader("PBRShader", "pbrvert.glsl", "pbrfrag.glsl", "", "", "");

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

  InitCamera();
  InitWorld();
}

Scene::~Scene() {
  delete cubeMesh;
  delete sphereMesh;
  delete cylinderMesh;
  delete coneMesh;

  delete physics;
  delete renderer;
  delete world;

  Assets::AssetManager::FlushAssets();
  InputManager::Dispose();
}

void Scene::UpdateGame(float dt) {


  //UpdateKeys();
  //SelectObject();
  //MoveSelectedObject();

  world->UpdateWorld(dt);
  renderer->Update(dt);
  physics->Update(dt);

  Debug::FlushRenderables();
  renderer->Render();
}

void Scene::UpdateKeys() {
	
}

void Scene::InitCamera() {
  //world->GetMainCamera()->SetNearPlane(3.0f);
  //world->GetMainCamera()->SetFarPlane(4200.0f);
  //world->GetMainCamera()->SetPitch(-10.0f);
  //world->GetMainCamera()->SetYaw(250.0f);
  //world->GetMainCamera()->SetPosition(Vector3(-50, 120, 200));
}

void Scene::InitWorld() {
  world->ClearAndErase();
}

GameObject* Scene::AddSphereToWorld(const Vector3& position, float radius, float mass, float restitution, float friction) {
  GameObject* sphere = new GameObject();

  sphere->GetTransform().SetWorldScale(Vector3(radius, radius, radius));
  sphere->GetTransform().SetWorldPosition(position);
  sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), ShapeType::sphere, mass, restitution, friction));
  sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicMaterial));

  world->AddGameObject(sphere);
  return sphere;
}

GameObject* Scene::AddCubeToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction) {
  GameObject* cube = new GameObject();

  cube->GetTransform().SetWorldScale(dimensions);
  cube->GetTransform().SetWorldPosition(position);
  cube->GetTransform().SetLocalOrientation(orient);
  cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), ShapeType::cube, mass, restitution, friction));
  cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicMaterial));
  cube->GetRenderObject()->SetMaterialInstanced();

  world->AddGameObject(cube);
  return cube;
}

void Scene::InitMixedGridWorld(const Vector3& positiony, int numRows, int numCols, float rowSpacing, float colSpacing) {
	for (int i = 0; i < numCols; ++i) {
		for (int j = 0; j < numRows; ++j) {
			float sphereRadius = 0.5 + 3.0 * (rand() % 100) / (float)100;
			float x = 1 + 10.0 * (rand() % 100) / (float)100;
			float y = 1 + 10.0 * (rand() % 100) / (float)100;
			float z = 1 + 10.0 * (rand() % 100) / (float)100;
			Vector3 cubeDims = Vector3(x, y, z);
			Vector3 position = Vector3(i * colSpacing, 15 + positiony.y * ((rand() % 100) / (float)100), j * rowSpacing);
			if (rand() % 2) {
				tempTex = dogsTex;
			}
			else {
				tempTex = dogTex;
			}
			if (rand() % 2) {
				GameObject* go = AddCubeToWorld(position, Quaternion::AxisAngleToQuaternion(Vector3((rand() % 100) / (float)100, (rand() % 100) / (float)100, (rand() % 100) / (float)100), rand() % 45), cubeDims, 10, (rand() % 100) / (float)100, (rand() % 100) / (float)100);
			}
			else {
				AddSphereToWorld(position, sphereRadius, 10, (rand() % 100) / (float)100, (rand() % 100) / (float)100);
			}
		}
	}
}
