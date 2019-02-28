#include "Resource.h"
#include "../TeamProject/InputManager.h"


Resource::Resource() {};

Resource::Resource(GameObject * obj) : ScriptObject(obj)
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
		gameObject->GetTransform().SetWorldPosition(pos);
		gameObject->GetPhysicsObject()->SetPosition(pos);
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
	gameObject->SetTag(LayerAndTag::Tags::Occupied);
	SetTarget(obj);
}

void Resource::Reset() 
{
	gameObject->SetTag(LayerAndTag::Tags::Resources);
	gameObject->GameObject::SetParent(GameObject::FindGameObjectWithTag(LayerAndTag::Tags::ResourceParent));
	moveSpeed = 100.0f;
	minDistance = 50.0f;
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

