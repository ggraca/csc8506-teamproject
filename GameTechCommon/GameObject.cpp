#include "GameObject.h"
#include "CollisionDetection.h"

using namespace NCL::CSC8503;

GameObject::GameObject(string objectName)	
{
	
	name			= objectName;
	isActive		= true;
	boundingVolume	= nullptr;
	physicsObject	= nullptr;
	renderObject	= nullptr;
	networkObject	= nullptr;
	layer			= LayerAndTag::ObjectLayer::Default;
	tag				= LayerAndTag::Tags::Untagged;
	
}

GameObject::~GameObject()	
{
	delete boundingVolume;
	delete physicsObject;
	delete renderObject;
	delete networkObject;

	ClearScripts();
	std::cout << name << " Destroyed" << std::endl;
}

void GameObject::ClearScripts()
{
	for (auto&i : scripts)
	{
		delete i;
	}
	scripts.clear();
}

bool GameObject::InsideAABB(const Vector3& boxPos, const Vector3& halfSize) {
	if (!boundingVolume) {
		return false;
	}
	return CollisionDetection::AABBTest(transform, *boundingVolume, boxPos, halfSize);
}

void GameObject::UpdateAttachedScripts()
{
	for (auto&i : scripts)
	{
		i->Update();
	}
}

void GameObject::LateUpdateAttachedScripts()
{
	for (auto&i : scripts)
	{
		i->LateUpdate();
	}
}


void GameObject::AddScript(GameObject* obj)
{
	if (obj) 
	{
		scripts.push_back(obj);
	}
		
}

