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
  physics->SetGravity(Vector3(0, -60.81, 0));

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

  /*cylinderMesh = new OGLMesh("cylinder.msh");
  cylinderMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
  cylinderMesh->UploadToGPU();*/

  basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
  brickTex = (OGLTexture*)TextureLoader::LoadAPITexture("dogs.jpg");
  woodTex = (OGLTexture*)TextureLoader::LoadAPITexture("wood1.jpg");
  wood2Tex = (OGLTexture*)TextureLoader::LoadAPITexture("wood2.jpg");
  grassTex = (OGLTexture*)TextureLoader::LoadAPITexture("grass.jpg");
  ballTex = (OGLTexture*)TextureLoader::LoadAPITexture("smileyface.png");
  dogTex = (OGLTexture*)TextureLoader::LoadAPITexture("doge.png");
  basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

  InitCamera();
  InitWorld();
}

Scene::~Scene() {
  delete cubeMesh;
  delete sphereMesh;
  delete basicTex;
  delete brickTex;
  delete woodTex;
  delete wood2Tex;
  delete grassTex;
  delete ballTex;
  delete dogTex;
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
  
  world->UpdateWorld(dt);
  renderer->Update(dt);
  physics->Update(dt);

  Debug::FlushRenderables();
  renderer->Render();
}

void Scene::UpdateKeys() {
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_O)) {
		btRigidBody::upcast(physics->dynamicsWorld->getCollisionObjectArray()[1])->setLinearVelocity(btVector3(10, 100, 0));
		
	}
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

void Scene::SetBulletPhysicsParameters(btCollisionShape* Shape, const Vector3& position, float mass1, float restitution, float friction, Quaternion orientation)
{
	physics->collisionShapes.push_back(Shape);
	btTransform Transform;
	btQuaternion orient = btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w);
	Transform.setIdentity();
	Transform.setOrigin(btVector3(position.x, position.y, position.z));
	Transform.setRotation(orient);
	btScalar mass(mass1);
	bool isDynamic = (mass != 0.0f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		Shape->calculateLocalInertia(mass, localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(Transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, Shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	physics->dynamicsWorld->addRigidBody(body);

	//body->setLinearVelocity(btVector3(10, 100, 0));
	//body->setAngularVelocity(btVector3(0, 10, 0));

	body->applyImpulse(btVector3(-1000, 1000, 0), btVector3(0, -5, 0));


	body->setFriction(friction);
	body->setRestitution(restitution);
	body->setRollingFriction(0.9);
	body->setSpinningFriction(0.3);
}

void Scene::AddSphereToWorld(OGLTexture* sphereTex, const Vector3& position, float radius, float mass, float restitution, float friction, Vector4 colour) {
  GameObject* sphere = new GameObject();

  btCollisionShape* Shape = new btSphereShape(btScalar(radius));
  SetBulletPhysicsParameters(Shape, position, mass, restitution, friction);

  //SphereVolume* volume = new SphereVolume(radius);
  //sphere->SetBoundingVolume((CollisionVolume*)volume);
  //sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume())); //TODO These 3 lines may still be required until we find some better way of linking render objects to physics objects

  Vector3 sphereSize = Vector3(radius, radius, radius);
  sphere->GetTransform().SetWorldScale(sphereSize);
  sphere->GetTransform().SetWorldPosition(position);
  sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, sphereTex, basicShader));
  sphere->GetRenderObject()->SetColour(colour);

  world->AddGameObject(sphere);

//  return sphere;
}

void Scene::AddCubeToWorld(OGLTexture* cubeTex, const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction, Vector4 colour) {
  GameObject* cube = new GameObject();

  cube->GetTransform().SetLocalOrientation(orient);
  cube->GetTransform().SetWorldPosition(position);
  cube->GetTransform().SetWorldScale(dimensions);

  cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, cubeTex, basicShader));
  cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

  world->AddGameObject(cube);
}

void Scene::InitMixedGridWorld(const Vector3& positiony, int numRows, int numCols, float rowSpacing, float colSpacing) {
	for (int i = 0; i < numCols; ++i) {
		for (int j = 0; j < numRows; ++j) {
			float sphereRadius = 0.5 + 3.0 * (rand() % 100) / (float)100;
			float x = 1 + 10.0 * (rand() % 100) / (float)100;
			float y = 1 + 10.0 * (rand() % 100) / (float)100;
			float z = 1 + 10.0 * (rand() % 100) / (float)100;
			Vector3 cubeDims = Vector3(x, y, z);
			Vector3 position = Vector3(i * colSpacing, 100 + positiony.y * ((rand() % 100) / (float)100), j * rowSpacing);
			if (rand() % 2) {
				tempTex = brickTex;
			}
			else {
				tempTex = dogTex;
			}
			//Vector4 colour = Vector4((rand() % 100) / (float)100, (rand() % 100) / (float)100, (rand() % 100) / (float)100, 1);
			Vector4 colour = Vector4(1,1,1,1);
			if (rand() % 2) {
				AddCubeToWorld(tempTex, position, Quaternion::AxisAngleToQuaterion(Vector3((rand() % 100) / (float)100, (rand() % 100) / (float)100, (rand() % 100) / (float)100), rand() % 45), cubeDims, 10, 0.4 + (rand() % 60) / (float)100, (rand() % 100) / (float)100, colour);
			}
			else {
				AddSphereToWorld(ballTex, position, sphereRadius, 10, (rand() % 100) / (float)100, 0.2 + (rand() % 80) / (float)100);
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
