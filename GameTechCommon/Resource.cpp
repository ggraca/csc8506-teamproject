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
	
}

void Resource::OnCollisionEnd(GameObject * otherObject)
{
}

void Resource::Aquire(GameObject * obj) 
{
	GameObject * captures = GameObject::FindGameObjectWithTag(LayerAndTag::Tags::Captures);
	gameObject->GameObject::SetParent(captures);
	gameObject->GetRenderObject()->SetColour(obj->GetRenderObject()->GetColour()); 
	gameObject->SetTag(LayerAndTag::Tags::Occupied);
	//set target
	obj->SetTarget(obj);
	
	if ((gameObject->GetTransform().GetLocalPosition().Length - obj->GetTransform().GetLocalPosition()).Length > minDistance)
	{
		auto direction = ((obj->GetTransform().GetLocalPosition() - gameObject->GetTransform().GetLocalPosition())).Normalised;
		auto amount = direction * moveSpeed;
		auto pos = (gameObject->GetTransform().GetLocalPosition());
		pos += amount;
		gameObject->GetTransform().GetLocalPosition() = pos;

	}

}



void Resource::Reset() 
{
	gameObject->SetTag(LayerAndTag::Tags::Resources);
	gameObject->GameObject::IsParent;
}
