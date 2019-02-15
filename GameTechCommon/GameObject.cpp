#include "GameObject.h"
#include "CollisionDetection.h"
#include "../TeamProject/InputManager.h"

using namespace NCL;
using namespace CSC8503;

GameWorld * GameObject::gameWorld = nullptr;

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

void GameObject::OnCollisionBegin(GameObject * otherObject)
{
	if (HasOtherScriptsAttached())
	{
		for (auto&i : scripts)
		{
			i->OnCollisionBegin(otherObject);
		}
	}
}

void GameObject::OnCollisionEnd(GameObject * otherObject)
{
	if (HasOtherScriptsAttached())
	{
		for (auto&i : scripts)
		{
			i->OnCollisionEnd(otherObject);
		}
	}
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

void GameObject::UpdateAttachedScripts(float dt)
{
	if (!HasOtherScriptsAttached()) { return; }

	for (auto&i : scripts)
	{
		i->Update(dt);
	}
}

void GameObject::LateUpdateAttachedScripts(float dt)
{
	if (!HasOtherScriptsAttached()) { return; }
	
	for (auto&i : scripts)
	{
		i->LateUpdate(dt);
	}
}

void GameObject::SetGameWorld(GameWorld * world)
{
	gameWorld = world;
}

GameObject * GameObject::Find(string name)
{
	if (!gameWorld) { return nullptr; }

	return gameWorld->Find(name);
}

GameObject * GameObject::FindGameObjectWithTag(LayerAndTag::Tags tag)
{
	if (!gameWorld) { return nullptr; }

	return gameWorld->FindGameObjectWithTag(tag);
}

vector<GameObject*> GameObject::FindGameObjectsWithTag(LayerAndTag::Tags tag)
{
	if (!gameWorld) { return vector<GameObject*>(); }

	return gameWorld->FindGameObjectsWithTag(tag);
}

vector<GameObject*> GameObject::GetChildrenOfObject(const GameObject * obj)
{
	if (!gameWorld) { return vector<GameObject*>(); }

	return gameWorld->GetChildrenOfObject(obj);
}

vector<GameObject*> GameObject::GetChildrenOfObject(const GameObject * obj, LayerAndTag::Tags tag)
{
	if (!gameWorld) { return vector<GameObject*>(); }

	return gameWorld->GetChildrenOfObject(obj,tag);
}

void GameObject::Destroy(GameObject * obj)
{
	if (!gameWorld) { return; }

	return gameWorld->Destroy(obj);
}

///////////////////////////////////Script Object
ScriptObject::ScriptObject()
{
	gameObject = nullptr;
	inputManager = nullptr;
}


ScriptObject::ScriptObject(GameObject * go)
{
	this->gameObject = go;
	this->inputManager = nullptr;
}

ScriptObject::ScriptObject(GameObject * go, InputManager * im)
{
	this->gameObject = go;
	this->inputManager = im;
}

ScriptObject::~ScriptObject()
{
	//don"t delete gameobject as it may still meant to live after script is detached
}

void ScriptObject::Awake()
{
}

void ScriptObject::Start()
{
}

void ScriptObject::Update(float dt)
{

	
}

void ScriptObject::LateUpdate(float dt)
{
}

void ScriptObject::OnCollisionBegin(GameObject * otherObject)
{
}

void ScriptObject::OnCollisionEnd(GameObject * otherObject)
{
}
