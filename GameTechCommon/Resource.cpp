#include "Resource.h"
#include "../TeamProject/InputManager.h"


Resource::Resource(GameObject * gameObject):ScriptObject(gameObject)
{
	gameObject->SetTag(LayerAndTag::Tags::Resources);
}

Resource::Resource(GameObject * gameObject, InputManager * im):ScriptObject(gameObject,im)
{
}

Resource::~Resource()
{
}


void Resource::Awake()
{
}

void Resource::Start()
{
}

void Resource::Update(float dt)
{
}

void Resource::LateUpdate(float dt)
{
}

void Resource::OnCollisionBegin(GameObject * otherObject)
{
	if(otherObject->CompareTag(LayerAndTag::Player))
	{
		//TODO Acquire resource
	}
}

void Resource::OnCollisionEnd(GameObject * otherObject)
{
}

void Resource::Aquire(GameObject * gameObject) {
	Dummy = new GameObject;
	gameObject->GameObject::SetParent(Dummy);
}
