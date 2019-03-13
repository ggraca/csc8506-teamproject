#include "GameWorld.h"
#include "GameObject.h"
#include <algorithm>
#include "BulletPhysics.h"

using namespace NCL;
using namespace NCL::CSC8503;

GameWorld::GameWorld()	{
	InitCamera();
	layering = LayerAndTag();
}

void GameWorld::InitCamera()
{
	cameraOffset = Vector3(0, 70, -150);

	mainCamera = new GameObject();
	mainCamera->AddComponent<CameraControl*>((Component*)new CameraControl(mainCamera));

	Transform * child = new Transform();
	child->SetLocalPosition(cameraOffset);
	child->SetLocalOrientation(Quaternion::EulerAnglesToQuaternion(-10,-180, 0));
	child->SetParent(&(mainCamera->GetTransform()));

	mainCamera->GetTransform().AddChild(child);
}

void GameWorld::SwitchToFPS()
{
	cameraOffset = Vector3(0, 0, 10);
	Transform * child = mainCamera->GetTransform().GetChildrenList()[0];

	if (child)
	{
		child->SetLocalPosition(cameraOffset);
		mainCamera->GetComponent<CameraControl*>()->SetCameraType(false);
	}
}

void GameWorld::SwitchToTPS()
{
	cameraOffset = Vector3(0, 70, -150);
	Transform * child = mainCamera->GetTransform().GetChildrenList()[0];

	if (child)
	{
		child->SetLocalPosition(cameraOffset);
		mainCamera->GetComponent<CameraControl*>()->SetCameraType(true);
	}
}

GameWorld::~GameWorld()	{
}

GameObject * GameWorld::Find(string name)
{

	for (auto&i : gameObjects)
	{
		if (i->GetName() == name) { return i; }
	}

	return nullptr;
}

GameObject * GameWorld::FindGameObjectWithTag(LayerAndTag::Tags tag)
{
	for (auto&i : gameObjects)
	{
		if (i->CompareTag(tag)) { return i; }
	}

	return nullptr;
}

vector<GameObject*> GameWorld::FindGameObjectsWithTag(LayerAndTag::Tags tag)
{
	vector<GameObject*> temp;

	for (auto&i : gameObjects)
	{
		if (i->CompareTag(tag)) { temp.push_back(i); }
	}

	return temp;
}

void GameWorld::LateDestroy(GameObject * obj) {
	objectsToDestroy.push_back(obj);
}

void GameWorld::Destroy(GameObject * obj)
{
	auto children = GetChildrenOfObject(obj);

	for (auto&i : children)
	{
		Destroy(i);
	}

	RemoveCollisionsFromGameObject(obj);
	RemoveGameObject(obj);
}

void GameWorld::HandleObjectsToDestroy() {
	for (auto go : objectsToDestroy) {
		Destroy(go);
	}

	objectsToDestroy.clear();
}

void GameWorld::Clear() {
	gameObjects.clear();
}

void GameWorld::ClearAndErase() {
	for (auto& i : gameObjects) {
		delete i;
	}
	Clear();
}

void GameWorld::LateInstantiate(GameObject * obj)
{
	objectsToInstantiate.push_back(obj);
}

void GameWorld::HandleObjectsToInstantiate()
{
	for (auto&i : objectsToInstantiate)
	{
		Instantiate(i);
	}

	objectsToInstantiate.clear();
}

void GameWorld::UpdateGameObjects(float dt)
{
	for (auto&i : gameObjects) 
	{	
		i->UpdateComponents(dt);
	}
}

void GameWorld::LateUpdateGameObjects(float dt)
{
	for (auto&i : gameObjects)
	{
		i->LateUpdateAttachedScripts(dt);
	}
}

void GameWorld::Instantiate(GameObject* o)
{
	if (!o) { return; }
	
	CallInitialObjectFunctions(o);
	gameObjects.push_back(o);

	PhysicsObject* pc = o->GetComponent<PhysicsObject*>();
  
	if (pc)
  {
		btCollisionShape* po = pc->GetShape();
		physics->collisionShapes.push_back(po);

		btRigidBody* pb = pc->GetRigidbody();
		physics->dynamicsWorld->addRigidBody(pb);
	}
}

void GameWorld::CallInitialObjectFunctions(NCL::CSC8503::GameObject * o)
{
	if (!o) { return; }

	o->SetIsAddedToWorld(true);
	o->SetUpInitialScripts();	
}

void GameWorld::Instantiate(GameObject* o, GameObject* parent )
{
	if (!o) { return; }

	o->SetParent(parent);
	Instantiate(o);
}

void GameWorld::RemoveGameObject(GameObject* o) 
{
	if (!o) { return; }

	o->SetParent(nullptr);

	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i] == o)
		{
			delete gameObjects[i];
			gameObjects.erase(gameObjects.begin() + i);
			o = nullptr;
			return;
		}
	}
}

void GameWorld::RemoveCollisionsFromGameObject(GameObject* obj) {
	for (auto collidingGo : obj->collidingObjects) {
		collidingGo->collidingObjects.erase(
			remove(
				collidingGo->collidingObjects.begin(),
				collidingGo->collidingObjects.end(),
				obj),
			collidingGo->collidingObjects.end()
		);
	}

	obj->collidingObjects.clear();
}


void GameWorld::GetObjectIterators(
	std::vector<GameObject*>::const_iterator& first,
	std::vector<GameObject*>::const_iterator& last) const {

	first	= gameObjects.begin();
	last	= gameObjects.end();
}

GameObject* GameWorld::GetPlayerGameObject()
{
	return gameObjects.at(1);
}

vector<GameObject*> GameWorld::GetGameObjectList()
{
  return gameObjects;
}

int GameWorld::GetObjectCount(){
	return (int) gameObjects.size();
}

void GameWorld::UpdateWorld(float dt) 
{
	UpdateGameObjects(dt);
	UpdateTransforms();
	LateUpdateGameObjects(dt);
	HandleObjectsToDestroy();
	HandleObjectsToInstantiate();
	mainCamera->GetComponent<CameraControl*>()->Update(dt);
}

vector<GameObject*> GameWorld::GetChildrenOfObject(GameObject* obj)
{
	vector<GameObject*> temp;

	for (auto& i : gameObjects)
	{
		if (i->IsParent(&obj->GetTransform()))
		{
			temp.emplace_back(i);
		}
	}

	return temp;
}

vector<GameObject*> GameWorld::GetChildrenOfObject(GameObject* obj,LayerAndTag::Tags tag)
{
	vector<GameObject*> temp;

	
	for (auto& i : gameObjects)
	{
		if (i->CompareTag(tag) && i->IsParent(&obj->GetTransform()))
		{
			temp.emplace_back(i);
		}
	}


	return temp;
}


void GameWorld::UpdateTransforms() {
	for (auto& i : gameObjects)
	{
		i->GetTransform().UpdateMatrices();
	}
}