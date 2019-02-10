#include "Scene.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/TextureLoader.h"

#include "../GameTechCommon/GameWorld.h"
#include "../GameTechCommon/PositionConstraint.h"

using namespace NCL;
using namespace CSC8503;

Scene::Scene() {
  world = new GameWorld();
  renderer = new GameTechRenderer(*world);
  physics = new BulletPhysics(*world);
  physics->gravity = Vector3(4, -18.81, 0);

  Debug::SetRenderer(renderer);

  InitialiseAssets();
}

void Scene::InitialiseAssets() {
  cubeMesh = new OGLMesh("Cube.msh");
  cubeMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
  cubeMesh->UploadToGPU();

  sphereMesh = new OGLMesh("sphere.msh");
  sphereMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
  sphereMesh->UploadToGPU();

  /*cylinderMesh = new OGLMesh("cylinder.msh");
  cylinderMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
  cylinderMesh->UploadToGPU();*/

  basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
  woodTex = (OGLTexture*)TextureLoader::LoadAPITexture("wood1.jpg");
  grassTex = (OGLTexture*)TextureLoader::LoadAPITexture("grass.jpg");
  ballTex = (OGLTexture*)TextureLoader::LoadAPITexture("smileyface.png");
  basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

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
  delete basicShader;

  delete physics;
  delete renderer;
  delete world;
}

void Scene::UpdateGame(float dt) {
  if (!inSelectionMode) {
    world->GetMainCamera()->UpdateCamera(dt);
  }

  UpdateKeys();

  SelectObject();
  MoveSelectedObject();

  world->UpdateWorld(dt);
  renderer->Update(dt);
  physics->Update(dt);

  Debug::FlushRenderables();
  renderer->Render();
}

void Scene::UpdateKeys() {
	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_J)) {
	//	Vector3 gravity = physics->gravity;
	//	physics->gravity = Vector3(gravity.x - 1, gravity.y, gravity.z);
	////	physics->gravity = Vector3(0, 0, 0);
	//}
	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_L)) {
	//	Vector3 gravity = physics->gravity;
	//	physics->gravity = Vector3(gravity.x + 1, gravity.y, gravity.z);
	//}
	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_J)) {
	//	Vector3 gravity = physics->gravity;
	//	physics->gravity = Vector3(gravity.x, gravity.y, gravity.z - 1);
	//}
	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_L)) {
	//	Vector3 gravity = physics->gravity;
	//	physics->gravity = Vector3(gravity.x, gravity.y, gravity.z + 1);
	//}
	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_P)) {
	//	Vector3 gravity = physics->gravity;
	//	physics->gravity = Vector3(gravity.x, gravity.y + 1, gravity.z - 1);
	//}
	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_O)) {
	//	Vector3 gravity = physics->gravity;
	//	physics->gravity = Vector3(gravity.x, gravity.y - 1, gravity.z + 1);
	//}
}

void Scene::InitCamera() {
  world->GetMainCamera()->SetNearPlane(3.0f);
  world->GetMainCamera()->SetFarPlane(4200.0f);
  world->GetMainCamera()->SetPitch(-35.0f);
  world->GetMainCamera()->SetYaw(320.0f);
  world->GetMainCamera()->SetPosition(Vector3(-50, 120, 200));
}

void Scene::InitWorld() {
  world->ClearAndErase();
}

void Scene::SetBulletPhysicsParameters(btCollisionShape* Shape, const Vector3& position, float inverseMass, Quaternion orientation)
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
}

GameObject* Scene::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
  GameObject* sphere = new GameObject();

  btCollisionShape* Shape = new btSphereShape(btScalar(radius));
  SetBulletPhysicsParameters(Shape, position, inverseMass);

  //SphereVolume* volume = new SphereVolume(radius);
  //sphere->SetBoundingVolume((CollisionVolume*)volume);
  //sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume())); //TODO These 3 lines may still be required until we find some better way of linking render objects to physics objects

  Vector3 sphereSize = Vector3(radius, radius, radius);
  sphere->GetTransform().SetWorldScale(sphereSize);
  sphere->GetTransform().SetWorldPosition(position);
  sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, ballTex, basicShader));
 
  world->AddGameObject(sphere);

  return sphere;
}

GameObject* Scene::AddCubeToWorld(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float inverseMass) {
  GameObject* cube = new GameObject();

  btCollisionShape* Shape = new btBoxShape(btVector3(btScalar(dimensions.x), btScalar(dimensions.y), btScalar(dimensions.z)));
  SetBulletPhysicsParameters(Shape, position, inverseMass, orient);

  //AABBVolume* volume = new AABBVolume(dimensions);
  //cube->SetBoundingVolume((CollisionVolume*)volume);
  //cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume())); //TODO These 3 lines may still required until we find some better way of linking render objects to physics objects
  
  cube->GetTransform().SetLocalOrientation(orient);
  cube->GetTransform().SetWorldPosition(position); 
  cube->GetTransform().SetWorldScale(dimensions);
  cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, woodTex, basicShader));
  
  world->AddGameObject(cube);

  return cube;
}

void Scene::InitMixedGridWorld(const Vector3& positiony, int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 3.0 * (rand() % 100) / (float)100;
	float x = 10.0 * (rand() % 100) / (float)100;
	srand(time(NULL));
	float y = 10.0 * (rand() % 100) / (float)100;
	srand(time(NULL));
	float z = 10.0 * (rand() % 100) / (float)100;
	srand(time(NULL));
	Vector3 cubeDims = Vector3(x, y, z);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			cout << (rand() % 100) / (float)100 << endl;
			Vector3 position = Vector3(x * colSpacing, positiony.y * ((rand() % 100) / (float)100), z * rowSpacing);
			if (rand() % 2) {
				AddCubeToWorld(position, Quaternion::AxisAngleToQuaterion(Vector3((rand() % 100) / (float)100, (rand() % 100) / (float)100, (rand() % 100) / (float)100), rand() % 45), cubeDims);
			}
			else {
				AddSphereToWorld(position, sphereRadius);
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
        selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
        selectionObject = nullptr;
      }

      Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

      RayCollision closestCollision;
      if (world->Raycast(ray, closestCollision, true)) {
        selectionObject = (GameObject*)closestCollision.node;
        selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
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
