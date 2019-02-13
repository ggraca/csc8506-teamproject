#include "GameObject.h"
#include "CollisionDetection.h"

using namespace NCL;
using namespace CSC8503;

GameObject::GameObject(string objectName)	
{
	
	name			= objectName;
	isActive		= true;
	isAddedToWorld  = false;
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

void GameObject::AddScript(ScriptObject * obj)
{
	if (!obj) { return; }
	
	scripts.push_back(obj);

	if (isAddedToWorld)
	{
		obj->Awake();
		obj->Start();
	}

	
}


void GameObject::SetUpInitialScripts()
{
	for (auto&i : scripts)
	{
		i->Awake();
		i->Start();
	}
}

void GameObject::UpdateAttachedScripts()
{
	if (!HasOtherScriptsAttached()) { return; }

	for (auto&i : scripts)
	{
		i->Update();
	}
}

void GameObject::LateUpdateAttachedScripts()
{
	if (!HasOtherScriptsAttached()) { return; }
	
	for (auto&i : scripts)
	{
		i->LateUpdate();
	}
}


ScriptObject::ScriptObject()
{
	gameObject = nullptr;
}

///////////////////////////////////Script Object
ScriptObject::ScriptObject(GameObject * go)
{
	this->gameObject = go;
}
