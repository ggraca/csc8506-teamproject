#include "Scene.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/Assets.h"
#include "GameWorld.h"

using namespace NCL;
using namespace CSC8503;

Scene::Scene() {
  world = new GameWorld();
  renderer = new GameTechRenderer(*world);
  physics = new BulletPhysics(*world);
  physics->SetGravity(Vector3(-4, -60.81, 0));

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

  /*cylinderMesh = new OGLMesh("cylinder.msh");
  cylinderMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
  cylinderMesh->UploadToGPU();*/

  basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
  woodTex = (OGLTexture*)TextureLoader::LoadAPITexture("wood1.jpg");
  healthBarGreen = (OGLTexture*)TextureLoader::LoadAPITexture("healthBarGreen.png");
  healthBarRed = (OGLTexture*)TextureLoader::LoadAPITexture("healthBarRed.png");
  grassTex = (OGLTexture*)TextureLoader::LoadAPITexture("grass.jpg");
  ballTex = (OGLTexture*)TextureLoader::LoadAPITexture("goal.jpg");
  //Old functions to show as comparison
  //pbrWoodDiff = (OGLTexture*)TextureLoader::LoadAPITexture("WoodPlanks/Wood_planks_COLOR.jpg");
  //pbrWoodBump = (OGLTexture*)TextureLoader::LoadAPITexture("WoodPlanks/Wood_planks_NORM.jpg");
  //pbrWoodSpec = (OGLTexture*)TextureLoader::LoadAPITexture("WoodPlanks/Wood_planks_DISP.jpg");
  //pbrWoodMet = (OGLTexture*)TextureLoader::LoadAPITexture("WoodPlanks/Wood_planks_SPEC.jpg");
  pbrWoodDiff = (OGLTexture*)Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_COLOR.jpg");
  pbrWoodBump = (OGLTexture*)Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_NORM.jpg");
  pbrWoodSpec = (OGLTexture*)Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_DISP.jpg");
  pbrWoodMet = (OGLTexture*)Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_SPEC.jpg");
 
  basicShader = new OGLShader("pbrvert.glsl", "pbrfrag.glsl");
  //basicShader = new OGLShader("pbrverttemp.glsl", "pbrfragtemp.glsl");
  renderer->healthBarGreen = healthBarGreen->GetObjectID();
  renderer->healthBarRed = healthBarRed->GetObjectID();

  basicMaterial = new Material();
  basicMaterial->SetShader(basicShader);
  basicMaterial->AddTextureParameter("diffuseTex", pbrWoodDiff);
  basicMaterial->AddTextureParameter("bumpTex", pbrWoodBump);
  basicMaterial->AddTextureParameter("specularTex", pbrWoodSpec);
  basicMaterial->AddTextureParameter("metalnessTex", pbrWoodMet);

  floorMat = new Material();
  floorMat->SetShader(basicShader);
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
  renderer->skybox = cubeMap->GetObjectID();

  InitCamera();
  InitWorld();
}

Scene::~Scene() {
  delete cubeMesh;
  delete sphereMesh;
  delete basicTex;
  delete woodTex;
  delete grassTex;
  delete ballTex;
  delete healthBarGreen;
  delete healthBarRed;
  delete basicShader;
  delete basicMaterial;

  delete physics;
  delete renderer;
  delete world;

  Assets::AssetManager::FlushAssets();
}

void Scene::UpdateGame(float dt) {
  if (!inSelectionMode) {
    world->GetMainCamera()->UpdateCamera(dt);
  }

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
  world->GetMainCamera()->SetNearPlane(3.0f);
  world->GetMainCamera()->SetFarPlane(4200.0f);
  world->GetMainCamera()->SetPitch(-10.0f);
  world->GetMainCamera()->SetYaw(250.0f);
  world->GetMainCamera()->SetPosition(Vector3(-50, 120, 200));
}

void Scene::InitWorld() {
  world->ClearAndErase();
}

void Scene::SetBulletPhysicsParameters(btCollisionShape* Shape, const Vector3& position, float inverseMass, float restitution, float friction, Quaternion orientation)
{
	physics->collisionShapes.push_back(Shape);
	btTransform Transform;
	btQuaternion orient = btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w);
	Transform.setIdentity();
	Transform.setOrigin(btVector3(position.x, position.y, position.z));
	Transform.setRotation(orient);
	btScalar mass(inverseMass);
	bool isDynamic = (mass != 0.0f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		Shape->calculateLocalInertia(mass, localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(Transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, Shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	physics->dynamicsWorld->addRigidBody(body);
	body->setFriction(0.4); //TODO Now physical properties can be set per object by passing parameters to this function from AddSphereToWorld/AddCubeToWorld, with default values when omitted. Will sort this v. soon!
	body->setRestitution(0.9);
	body->setRollingFriction(0.9);
	body->setSpinningFriction(0.3);
}

GameObject* Scene::AddSphereToWorld(const Vector3& position, float radius, float inverseMass, float restitution, float friction) {
  GameObject* sphere = new GameObject();

  btCollisionShape* Shape = new btSphereShape(btScalar(radius));
  SetBulletPhysicsParameters(Shape, position, inverseMass, restitution, friction);

  //SphereVolume* volume = new SphereVolume(radius);
  //sphere->SetBoundingVolume((CollisionVolume*)volume);
  //sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume())); //TODO These 3 lines may still be required until we find some better way of linking render objects to physics objects

  Vector3 sphereSize = Vector3(radius, radius, radius);
  sphere->GetTransform().SetWorldScale(sphereSize);
  sphere->GetTransform().SetWorldPosition(position);
  sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicMaterial));

  world->AddGameObject(sphere);

  return sphere;
}

GameObject* Scene::AddCubeToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float inverseMass, float restitution, float friction) {
  GameObject* cube = new GameObject();

  btCollisionShape* Shape = new btBoxShape(btVector3(btScalar(dimensions.x), btScalar(dimensions.y), btScalar(dimensions.z)));
  SetBulletPhysicsParameters(Shape, position, inverseMass, restitution, friction, orient);

  //AABBVolume* volume = new AABBVolume(dimensions);
  //cube->SetBoundingVolume((CollisionVolume*)volume);
  //cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume())); //TODO These 3 lines may still required until we find some better way of linking render objects to physics objects

  cube->GetTransform().SetLocalOrientation(orient);
  cube->GetTransform().SetWorldPosition(position);
  cube->GetTransform().SetWorldScale(dimensions);
  cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicMaterial));
  cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

  cube->GetPhysicsObject()->SetInverseMass(inverseMass);
  cube->GetPhysicsObject()->InitCubeInertia();

  world->AddGameObject(cube);


  return cube;
}

GameObject* Scene::AddFloorToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float inverseMass, float restitution, float friction) {
	GameObject* cube = new GameObject();

	btCollisionShape* Shape = new btBoxShape(btVector3(btScalar(dimensions.x), btScalar(dimensions.y), btScalar(dimensions.z)));
	SetBulletPhysicsParameters(Shape, position, inverseMass, restitution, friction, orient);

	//AABBVolume* volume = new AABBVolume(dimensions);
	//cube->SetBoundingVolume((CollisionVolume*)volume);
	//cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume())); //TODO These 3 lines may still required until we find some better way of linking render objects to physics objects

	cube->GetTransform().SetLocalOrientation(orient);
	cube->GetTransform().SetWorldPosition(position);
	cube->GetTransform().SetWorldScale(dimensions);
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, floorMat));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

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
				AddCubeToWorld(position, Quaternion::AxisAngleToQuaterion(Vector3((rand() % 100) / (float)100, (rand() % 100) / (float)100, (rand() % 100) / (float)100), rand() % 45), cubeDims, 10, 10.0 * (rand() % 100) / (float)100);
			}
			else {
				AddSphereToWorld(position, sphereRadius, 10, 10.0 * (rand() % 100) / (float)100, 10.0 * (rand() % 100) / (float)100);
			}
		}
	}
}

bool Scene::SelectObject() {
  if (Window::GetKeyboard()->KeyPressed(KEYBOARD_Q)) {
    inSelectionMode = !inSelectionMode;
    if (inSelectionMode) {
      Window::GetWindow()->ShowOSPointer(true);
      Window::GetWindow()->LockMouseToWindow(false);
    }
    else {
      Window::GetWindow()->ShowOSPointer(false);
      Window::GetWindow()->LockMouseToWindow(true);
    }
  }
  if (inSelectionMode) {
    renderer->DrawString("Press Q to change to camera mode!", Vector2(10, 0));

    if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::MOUSE_LEFT)) {
      if (selectionObject) {	//set colour to deselected;
        selectionObject->GetRenderObject()->GetMaterial()->SetColour(Vector4(1, 1, 1, 1));
        selectionObject = nullptr;
      }

      Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

      RayCollision closestCollision;
      if (world->Raycast(ray, closestCollision, true)) {
        selectionObject = (GameObject*)closestCollision.node;
        selectionObject->GetRenderObject()->GetMaterial()->SetColour(Vector4(0, 1, 0, 1));
        return true;
      }
      else {
        return false;
      }
    }
  }
  else {
    renderer->DrawString("Press Q to change to select mode!", Vector2(10, 0));
  }
  return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/

void Scene::MoveSelectedObject() {
  renderer->DrawString("Click Force:" + std::to_string(forceMagnitude), Vector2(10, 20));
  forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

  if (!selectionObject) {
    return;//we haven't selected anything!
  }
  //Push the selected object!
  if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::MOUSE_RIGHT)) {
    Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

    RayCollision closestCollision;
    if (world->Raycast(ray, closestCollision, true)) {
      if (closestCollision.node == selectionObject) {
        selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
      }
    }
  }
}
