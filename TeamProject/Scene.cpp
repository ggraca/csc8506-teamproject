#include "Scene.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/TextureLoader.h"
#include "GameWorld.h"

using namespace NCL;
using namespace CSC8503;

Scene::Scene() {
  world = new GameWorld();
  renderer = new GameTechRenderer(*world);
  physics = new BulletPhysics(*world);
  physics->SetGravity(Vector3(-4, -60.81, 0));
  world->SetPhysics(physics);

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
//basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");
  basicShader = new OGLShader("pbrverttemp.glsl", "pbrfragtemp.glsl");

  InitCamera();
  InitWorld();
}

Scene::~Scene() {
  delete cubeMesh;
  delete sphereMesh;
  delete cylinderMesh;
  delete coneMesh;
  delete basicTex;
  delete woodTex;
  delete grassTex;
  delete ballTex;
  delete basicShader;

  delete physics;
  delete renderer;
  delete world;
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
  sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, ballTex, basicShader));
  sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), ShapeType::sphere, mass, restitution, friction));

  world->AddGameObject(sphere);
  return sphere;
}

GameObject* Scene::AddCubeToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction) {
  GameObject* cube = new GameObject();

  cube->GetTransform().SetWorldScale(dimensions);
  cube->GetTransform().SetWorldPosition(position);
  cube->GetTransform().SetLocalOrientation(orient);
  cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, woodTex, basicShader));
  cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), ShapeType::cube, mass, restitution, friction));

  world->AddGameObject(cube);
  return cube;
}

GameObject* Scene::AddCylinderToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction) {
  GameObject* cylinder = new GameObject();

  cylinder->GetTransform().SetWorldScale(dimensions);
  cylinder->GetTransform().SetWorldPosition(position);
  cylinder->GetTransform().SetLocalOrientation(orient);
  cylinder->SetRenderObject(new RenderObject(&cylinder->GetTransform(), cylinderMesh, woodTex, basicShader));
  cylinder->SetPhysicsObject(new PhysicsObject(&cylinder->GetTransform(), ShapeType::cylinder, mass, restitution, friction));

  world->AddGameObject(cylinder);
  return cylinder;
}

GameObject* Scene::AddConeToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction) {
  GameObject* cone = new GameObject();

  cone->GetTransform().SetWorldScale(dimensions);
  cone->GetTransform().SetWorldPosition(position);
  cone->GetTransform().SetLocalOrientation(orient);
  cone->SetRenderObject(new RenderObject(&cone->GetTransform(), coneMesh, woodTex, basicShader));
  cone->SetPhysicsObject(new PhysicsObject(&cone->GetTransform(), ShapeType::cone, mass, restitution, friction));

  world->AddGameObject(cone);
  return cone;
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
				go->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));  
				go->GetRenderObject()->SetDefaultTexture(tempTex);
			}
			else {
				AddSphereToWorld(position, sphereRadius, 10, (rand() % 100) / (float)100, (rand() % 100) / (float)100);
			}
		}
	}
}
