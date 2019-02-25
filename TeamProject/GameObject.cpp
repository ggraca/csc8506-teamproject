#include "GameObject.h"
#include "InputManager.h"

using namespace NCL;
using namespace CSC8503;

GameWorld * GameObject::gameWorld = nullptr;

GameObject::GameObject(string objectName)	
{
	
	name			= objectName;
	isActive		= true;
	isAddedToWorld  = false;
	boundingVolume	= nullptr;
	//physicsObject	= nullptr;
	renderObject	= nullptr;
	networkObject	= nullptr;
	layer			= LayerAndTag::ObjectLayer::Default;
	tag				= LayerAndTag::Tags::Untagged;
	
}

GameObject::~GameObject()	
{
	delete boundingVolume;
	//delete physicsObject;
	delete renderObject;
	delete networkObject;

	ClearComponents();
	ClearScripts();
	std::cout << name << " Destroyed" << std::endl;
}

void GameObject::ClearScripts()
{
	scripts.clear();
}

void GameObject::ClearComponents()
{
	for (auto&i : components)
	{
		delete i;
	}
	components.clear();
}

void GameObject::OnCollisionBegin(GameObject * otherObject)
{
	if (!HasOtherScriptsAttached()) { return; }
	
	for (auto&i : scripts)
	{
		i->OnCollisionBegin(otherObject);
	}
}

void GameObject::OnCollisionEnd(GameObject * otherObject)
{
	if (!HasOtherScriptsAttached()) { return; }

	for (auto&i : scripts)
	{
		i->OnCollisionEnd(otherObject);
	}
	
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

void GameObject::AddComponent(Component * obj)
{
	if (!obj) { return; }

	if (dynamic_cast<ScriptObject*>(obj)) { AddScript((ScriptObject*)dynamic_cast<ScriptObject*>(obj)); }

	components.push_back(obj);
}

void GameObject::SetUpInitialScripts()
{
	for (auto&i : scripts)
	{
		i->Awake();
		i->Start();
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

void GameObject::CallOnCollisionEnterForScripts(GameObject * otherObject)
{
	OnCollisionBegin(otherObject);

	if (!HasOtherScriptsAttached()) { return; }

	for (auto&i : scripts)
	{
		i->OnCollisionBegin(otherObject);
	}
}

void GameObject::CallOnCollisionEndForScripts(GameObject * otherObject)
{
	OnCollisionEnd(otherObject);

	if (!HasOtherScriptsAttached()) { return; }

	for (auto&i : scripts)
	{
		i->OnCollisionEnd(otherObject);
	}
}

void GameObject::UpdateComponents(float dt)
{
	
	if (!HasComponentsAttached()) { return; }

	for (auto&i : components)
	{
		i->Update(dt);
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

void GameObject::AddObjectToWorld(GameObject * obj)
{
	if (!gameWorld) { return; }
	
	gameWorld->AddGameObject(obj);
}

void GameObject::AddObjectToWorld(GameObject * obj, GameObject * parent)
{
	if (!gameWorld) { return; }

	gameWorld->AddGameObject(obj, parent);
}

GameObject * GameObject::GetMainCamera()
{
	if (!gameWorld) { return nullptr; }

	return gameWorld->GetMainCamera();
}




