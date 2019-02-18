#include "Resource.h"
#include "../TeamProject/InputManager.h"


Resource::Resource(GameObject * gameObject):ScriptObject(gameObject)
{
	Reset();
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
	FollowTarget(dt);
}

void Resource::FollowTarget(float &dt)
{
	if (target && (gameObject->GetTransform().GetWorldPosition() - target->GetTransform().GetWorldPosition()).Length() > minDistance)
	{
		auto direction = ((target->GetTransform().GetWorldPosition() - gameObject->GetTransform().GetWorldPosition())).Normalised();
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
	gameObject->GameObject::SetParent(GameObject::FindGameObjectWithTag(LayerAndTag::Tags::CaptureParent));
	gameObject->GetRenderObject()->SetColour(obj->GetRenderObject()->GetColour()); 
	gameObject->SetTag(LayerAndTag::Tags::Occupied);
	SetTarget(obj);
}

void Resource::Reset() 
{
	gameObject->SetTag(LayerAndTag::Tags::Resources);
	gameObject->GameObject::SetParent(GameObject::FindGameObjectWithTag(LayerAndTag::Tags::ResourceParent));
	gameObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
	moveSpeed = 5.0f;
	SetTarget(nullptr);

}

void Resource::SetTarget(GameObject * t)
{
	this->target = t;
}

GameObject* Resource::GetTarget() const
{
	return target;
}

