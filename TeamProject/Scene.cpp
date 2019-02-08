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
  physics = new PhysicsSystem(*world);

  

  forceMagnitude = 2000.0f;
  useGravity = false;
  inSelectionMode = false;

  Debug::SetRenderer(renderer);

  InitialiseAssets();
}

//Scene::Scene(float g) {
//	world = new GameWorld();
//	renderer = new GameTechRenderer(*world);
//	physics = new PhysicsSystem(*world);
//
//	bulletPhysics = new BulletPhysics(g);
//
//	forceMagnitude = 2000.0f;
//	useGravity = false;
//	inSelectionMode = false;
//
//	Debug::SetRenderer(renderer);
//
//	InitialiseAssets();
//}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes,
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it

*/
void Scene::InitialiseAssets() {
  cubeMesh = new OGLMesh("cube.msh");
  cubeMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
  cubeMesh->UploadToGPU();

  sphereMesh = new OGLMesh("sphere.msh");
  sphereMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
  sphereMesh->UploadToGPU();

  basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
  woodTex = (OGLTexture*)TextureLoader::LoadAPITexture("wood1.jpg");
  grassTex = (OGLTexture*)TextureLoader::LoadAPITexture("grass.jpg");
  ballTex = (OGLTexture*)TextureLoader::LoadAPITexture("goal.jpg");
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

  //delete physics;
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
  physics->Clear();
}

/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* Scene::AddFloorToWorld(const Vector3& position) {
  GameObject* floor = new GameObject();

  Vector3 floorSize = Vector3(500, 10, 500);
  AABBVolume* volume = new AABBVolume(floorSize);
  floor->SetBoundingVolume((CollisionVolume*)volume);
  floor->GetTransform().SetWorldScale(floorSize);
  floor->GetTransform().SetWorldPosition(position);

  floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, grassTex, basicShader));
  floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

  floor->GetPhysicsObject()->SetInverseMass(0);
  floor->GetPhysicsObject()->InitCubeInertia();

  world->AddGameObject(floor);

  return floor;
}

void Scene::SetBulletPhysicsParameters(btCollisionShape* Shape, const Vector3& position, float inverseMass)
{
	physics->bulletPhysics->collisionShapes.push_back(Shape);
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btVector3(position.x, position.y, position.z)); //TODO Cast position vector?
	btScalar mass(inverseMass);
	bool isDynamic = (mass != 0.0f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		Shape->calculateLocalInertia(mass, localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(Transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, Shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	physics->bulletPhysics->dynamicsWorld->addRigidBody(body);
}

GameObject* Scene::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
  GameObject* sphere = new GameObject();

  btCollisionShape* Shape = new btSphereShape(btScalar(radius));
  SetBulletPhysicsParameters(Shape, position, inverseMass);


  SphereVolume* volume = new SphereVolume(radius);
  sphere->SetBoundingVolume((CollisionVolume*)volume);
  sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume())); //TODO These 3 lines still required until we find some way of linking render objects to physics objects
  //sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
  //sphere->GetPhysicsObject()->InitSphereInertia();


  Vector3 sphereSize = Vector3(radius, radius, radius);
  sphere->GetTransform().SetWorldScale(sphereSize);
  sphere->GetTransform().SetWorldPosition(position);
  sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, ballTex, basicShader));
 
  world->AddGameObject(sphere);

  return sphere;
}



GameObject* Scene::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass) {
  GameObject* cube = new GameObject();

  btCollisionShape* Shape = new btBoxShape(btVector3(btScalar(dimensions.x), btScalar(dimensions.y), btScalar(dimensions.z)));
  SetBulletPhysicsParameters(Shape, position, inverseMass);



  AABBVolume* volume = new AABBVolume(dimensions);
  cube->SetBoundingVolume((CollisionVolume*)volume);
  cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume())); //TODO These 3 lines still required until we find some way of linking render objects to physics objects
  //cube->GetPhysicsObject()->SetInverseMass(inverseMass);
  //cube->GetPhysicsObject()->InitCubeInertia();



  cube->GetTransform().SetWorldPosition(position); //TODO Is this used for rendering purposes???
  cube->GetTransform().SetWorldScale(dimensions); //TODO Is this used for rendering purposes???
  cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, woodTex, basicShader));
  
  world->AddGameObject(cube);

  return cube;
}

/*

Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around.

*/
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
