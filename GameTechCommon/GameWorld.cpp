#include "GameWorld.h"
#include "GameObject.h"
#include "CollisionDetection.h"
#include "../Common/Camera.h"
#include <algorithm>

using namespace NCL;
using namespace NCL::CSC8503;

GameWorld::GameWorld()	{
	mainCamera = new Camera();

	quadTree = nullptr;
	shuffleConstraints	= false;
	shuffleObjects		= false;
	layering = LayerAndTag();
}

GameWorld::~GameWorld()	{
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

void GameWorld::AddGameObject(GameObject* o) 
{
	gameObjects.emplace_back(o);
}

void GameWorld::AddGameObject(GameObject* o,const GameObject* parent )
{
	o->SetParent(parent);
	gameObjects.emplace_back(o);
}

void GameWorld::RemoveGameObject(GameObject* o) 
{
	o->SetParent(nullptr);
	std::remove(gameObjects.begin(), gameObjects.end(), o);
}

void GameWorld::GetObjectIterators(
	std::vector<GameObject*>::const_iterator& first,
	std::vector<GameObject*>::const_iterator& last) const {

	first	= gameObjects.begin();
	last	= gameObjects.end();
}

void GameWorld::UpdateWorld(float dt) 
{
	UpdateTransforms();

	if (shuffleObjects) {
		std::random_shuffle(gameObjects.begin(), gameObjects.end());
	}

	if (shuffleConstraints) {
		std::random_shuffle(constraints.begin(), constraints.end());
	}
}

vector<GameObject*> GameWorld::GetChildrenOfObject(const GameObject* obj)
{
	vector<GameObject*> temp;

	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;
	GetObjectIterators(first, last);

	for (first; first < last; first++)
	{
		if ((*first)->IsParent(obj->GetRenderObject()->GetTransform()))
		{
			temp.push_back((*first));
		}
	}
	return temp;
}

vector<GameObject*> GameWorld::GetChildrenOfObject(const GameObject* obj,LayerAndTag::Tags tag)
{
	vector<GameObject*> temp;

	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;
	GetObjectIterators(first, last);

	for (first; first < last; first++)
	{
		if ((*first)->CompareTag(tag) && (*first)->IsParent(obj->GetRenderObject()->GetTransform()))
		{
			temp.push_back((*first));
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
