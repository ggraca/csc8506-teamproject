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
	if (target!=nullptr &&(gameObject->GetTransform().GetWorldPosition().Length - target->GetTransform().GetWorldPosition().Length) > minDistance)
	{
		auto direction = ((target->GetTransform().GetWorldPosition() - gameObject->GetTransform().GetWorldPosition())).Normalised;
		auto amount = direction * moveSpeed * dt;
		auto pos = (gameObject->GetTransform().GetWorldPosition());
		pos += amount;
		gameObject->GetTransform().GetWorldPosition() = pos;
	}
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
	SetTarget(obj);
}



void Resource::Reset() 
{
	gameObject->SetTag(LayerAndTag::Tags::Resources);
	gameObject->GameObject::SetParent(gameObject);
	gameObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
	target = nullptr;
}

void Resource::SetTarget(GameObject * t)
{
	this->target = t;
}

const Resource& GetTarget() const
{
	return target;
}

