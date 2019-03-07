#include "GameObject.h"
#include "ScriptObject.h"
#include "Component.h"
#include "GameWorld.h"
#include "RenderObject.h"

using namespace NCL;
using namespace CSC8503;

GameWorld * GameObject::gameWorld = nullptr;

GameObject::GameObject(std::string objectName)	
{
	name			= objectName;
	isActive		= true;
	isAddedToWorld  = false;
	layer			= LayerAndTag::ObjectLayer::Default;
	tag				= LayerAndTag::Tags::Untagged;
	transform.SetGameObject(this);
}

GameObject::~GameObject()	
{
	ClearComponents();
	std::cout << name << " Destroyed" << std::endl;
}

void GameObject::ClearComponents()
{
	for (auto&i : components)
	{
		if (i.second) { delete i.second; }
	}
	components.clear();
	scripts.clear();
}

void GameObject::SetParent(GameObject * parent)
{
	if (parent)
	{
		GetTransform().SetParent(&parent->GetTransform());
		GetTransform().AddChild(&parent->GetTransform());
	}
	else
	{
		if (GetTransform().GetParent() != nullptr)
		{
			GetTransform().GetParent()->RemoveChild(&parent->GetTransform());
		}

		GetTransform().SetParent(nullptr);
	}
}

bool GameObject::IsParent(const Transform* transform)
{
	return (GetTransform().GetParent() == transform);
}

void GameObject::AddChild(GameObject * child)
{
	child->SetParent(this);
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
	if (!HasOtherScriptsAttached()) { return; }

	for (auto&i : scripts)
	{
		i->OnCollisionBegin(otherObject);
	}
}

void GameObject::CallOnCollisionEndForScripts(GameObject * otherObject)
{
	if (!HasOtherScriptsAttached()) { return; }

	for (auto&i : scripts)
	{
		i->OnCollisionEnd(otherObject);
	}
}

void GameObject::UpdateComponents(float dt)
{
	for (auto&i : components)
	{
		if (components[i.first]) { i.second->Update(dt); }
	}
}

void GameObject::SetGameWorld(GameWorld * world)
{
	gameWorld = world;
}

GameObject * GameObject::Find(std::string name)
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

vector<GameObject*> GameObject::GetChildrenOfObject(GameObject * obj)
{
	if (!gameWorld) { return vector<GameObject*>(); }

	return gameWorld->GetChildrenOfObject(obj);
}

vector<GameObject*> GameObject::GetChildrenOfObject(GameObject * obj, LayerAndTag::Tags tag)
{
	if (!gameWorld) { return vector<GameObject*>(); }

	return gameWorld->GetChildrenOfObject(obj,tag);
}

void GameObject::Destroy(GameObject * obj)
{
	if (!gameWorld) { return; }

	return gameWorld->LateDestroy(obj);
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





