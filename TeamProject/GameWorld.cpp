#include "GameWorld.h"
#include "../TeamProject/GameObject.h"
#include "../GameTechCommon/CollisionDetection.h"
#include "../Common/Camera.h"
#include <algorithm>

using namespace NCL;
using namespace NCL::CSC8503;

GameWorld::GameWorld()	{
	InitCamera();

	quadTree = nullptr;
	shuffleConstraints	= false;
	shuffleObjects		= false;
	layering = LayerAndTag();
}

void NCL::CSC8503::GameWorld::InitCamera()
{
	mainCamera = new GameObject();
	mainCamera->AddScript((ScriptObject*)new CameraControl(mainCamera));
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

void GameWorld::Destroy(GameObject * obj)
{
	auto children = GetChildrenOfObject(obj);

	for (auto&i : children)
	{
		Destroy(i);
	}

	RemoveGameObject(obj);
}

void GameWorld::Clear() {
	gameObjects.clear();
	constraints.clear();
}

void GameWorld::ClearAndErase() {
	for (auto& i : gameObjects) {
		delete i;
	}
	for(auto& i : constraints) {
		delete i;
	}
	Clear();
}



void GameWorld::UpdateGameObjects(float dt)
{
	for (auto&i : gameObjects) 
	{	
		i->UpdateAttachedScripts(dt);
	}
}

void GameWorld::LateUpdateGameObjects(float dt)
{
	for (auto&i : gameObjects)
	{
		i->LateUpdateAttachedScripts(dt);
	}
}

void GameWorld::AddGameObject(GameObject* o)
{
	if (!o) { return; }
	
	CallInitialObjectFunctions(o);
	gameObjects.push_back(o);
	
}

void GameWorld::CallInitialObjectFunctions(NCL::CSC8503::GameObject * o)
{
	if (!o) { return; }

	o->SetIsAddedToWorld(true);
	o->SetUpInitialScripts();
	
}

void GameWorld::AddGameObject(GameObject* o,const GameObject* parent )
{
	if (!o) { return; }

	o->SetParent(parent);
	AddGameObject(o);
}

void GameWorld::RemoveGameObject(GameObject* o) 
{
	if (!o) { return; }

	o->SetParent(nullptr);

	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i] == o)
		{
			delete gameObjects[i];
			gameObjects.erase(gameObjects.begin() + i);
			o = nullptr;
		}
	}
}

void GameWorld::GetObjectIterators(
	std::vector<GameObject*>::const_iterator& first,
	std::vector<GameObject*>::const_iterator& last) const {

	first	= gameObjects.begin();
	last	= gameObjects.end();
}

int GameWorld::GetObjectCount(){
	return gameObjects.size();
}

void GameWorld::UpdateWorld(float dt) 
{
	UpdateGameObjects(dt);
	UpdateTransforms();
	LateUpdateGameObjects(dt);
	mainCamera->GetScript<CameraControl*>()->Update(dt);
}

vector<GameObject*> GameWorld::GetChildrenOfObject(const GameObject* obj)
{
	vector<GameObject*> temp;


	for (auto& i : gameObjects)
	{
		if (i->IsParent(obj->GetRenderObject()->GetTransform()))
		{
			temp.emplace_back(i);
		}
	}


	return temp;
}

vector<GameObject*> GameWorld::GetChildrenOfObject(const GameObject* obj,LayerAndTag::Tags tag)
{
	vector<GameObject*> temp;

	
	for (auto& i : gameObjects)
	{
		if (i->CompareTag(tag) && i->IsParent(obj->GetRenderObject()->GetTransform()))
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

void GameWorld::UpdateQuadTree() {
	delete quadTree;

	quadTree = new QuadTree<GameObject*>(Vector2(512, 512), 6);

	for (auto& i : gameObjects) {
		quadTree->Insert(i);
	}
}

bool GameWorld::Raycast(Ray& r, RayCollision& closestCollision, bool closestObject) const {
	//The simplest raycast just goes through each object and sees if there's a collision
	RayCollision collision;

	for (auto& i : gameObjects) {
		if (!i->GetBoundingVolume()) { //objects might not be collideable etc...
			continue;
		}
		RayCollision thisCollision;
		if (CollisionDetection::RayIntersection(r, *i, thisCollision)) {

			if (!closestObject) {
				closestCollision		= collision;
				closestCollision.node = i;
				return true;
			}
			else {
				if (thisCollision.rayDistance < collision.rayDistance) {
					thisCollision.node = i;
					collision = thisCollision;
				}
			}
		}
	}
	if (collision.node) {
		closestCollision		= collision;
		closestCollision.node	= collision.node;
		return true;
	}
	return false;
}


/*
Constraint Tutorial Stuff
*/

void GameWorld::AddConstraint(Constraint* c) {
	constraints.emplace_back(c);
}

void GameWorld::RemoveConstraint(Constraint* c) {
	std::remove(constraints.begin(), constraints.end(), c);
}

void GameWorld::GetConstraintIterators(
	std::vector<Constraint*>::const_iterator& first,
	std::vector<Constraint*>::const_iterator& last) const {
	first	= constraints.begin();
	last	= constraints.end();
}
