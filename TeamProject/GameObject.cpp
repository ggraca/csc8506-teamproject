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
	if (GetTransform().GetParent() != nullptr)
	{
		GetTransform().GetParent()->RemoveChild(&GetTransform());
	}
	if (parent)
	{
		GetTransform().SetParent(&parent->GetTransform());
		parent->GetTransform().AddChild(&GetTransform());
	}
	else
	{
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

GameObject * GameObject::GetMainCamera()
{
	if (!gameWorld) { return nullptr; }

	return gameWorld->GetMainCamera();
}

GameObject* GameObject::FromOBJ(OBJGeometry* obj) {
	if (!gameWorld) { return nullptr; }

	GameObject* root = new GameObject();
	gameWorld->LateInstantiate(root);

	for (auto& mesh : obj->GetChildren()) {
		GameObject* go = new GameObject();

		go->AddComponent<RenderObject*>(new RenderObject(
			&go->GetTransform(),
			mesh,
			((OBJMesh*)mesh)->material
		));

		gameWorld->LateInstantiate(go);
		root->AddChild(go);
	}
	return root;
}

void GameObject::AddRenderObject(string filename, GameObject* root, const Vector3& dim, const Vector3& pos, const Quaternion& orient, Material* mat) {
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ(filename);
	if (!gameWorld) { return; }
	
	for (auto& mesh : objGeometry->GetChildren()) {
		GameObject* go = new GameObject();
		if (mat == nullptr) {
			mat = ((OBJMesh*)mesh)->material;
		}
		go->AddComponent<RenderObject*>(new RenderObject(&go->GetTransform(), mesh, mat));
		root->AddChild(go);
	}
	
	GetTransform().SetWorldScale(dim);
	GetTransform().SetWorldPosition(pos);
	GetTransform().SetLocalOrientation(orient);

	gameWorld->LateInstantiateRecursively(root);
}
