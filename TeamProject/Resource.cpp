#include "Resource.h"
#include "../TeamProject/InputManager.h"


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
		gameObject->GetComponent<PhysicsObject*>()->SetPosition(pos);
	}
}

void Resource::LateUpdate(float dt)
{
}

void Resource::OnCollisionBegin(GameObject * otherObject)
{
	if (otherObject->CompareTag(LayerAndTag::Tags::Player)) {
		CAudioEngine* audio = gameObject->gameWorld->GetAudio();
		Vector3 pos = gameObject->GetTransform().GetWorldPosition();
		audio->PlayEvent("event:/Swords", pos);
//		int x = audio->PlaySounds(Assets::SOUNDSDIR + "swords.wav", pos, 10.0f);
//		audio->SetChannel3dPosition(x, pos);
	}
	if (otherObject->CompareTag(LayerAndTag::Tags::Resources)) {
		CAudioEngine* audio = gameObject->gameWorld->GetAudio();
		Vector3 pos = gameObject->GetTransform().GetWorldPosition();
		audio->PlayEvent("event:/Swords", pos);
//		int x = audio->PlaySounds(Assets::SOUNDSDIR + "bat.wav", pos, 10.0f);
//		audio->SetChannel3dPosition(x, pos);
	}
	if (otherObject->CompareTag(LayerAndTag::Tags::Occupied)) {
		CAudioEngine* audio = gameObject->gameWorld->GetAudio();
		Vector3 pos = gameObject->GetTransform().GetWorldPosition();
		audio->PlayEvent("event:/bomb", pos);
//		int x = audio->PlaySounds(Assets::SOUNDSDIR + "ole.wav", pos, 10.0f);
//		audio->SetChannel3dPosition(x, pos);
	}
}

void Resource::OnCollisionEnd(GameObject * otherObject)
{
}

void Resource::Aquire(GameObject * obj) 
{
	gameObject->GameObject::SetParent(GameObject::FindGameObjectWithTag(LayerAndTag::Tags::CaptureParent));
	gameObject->SetTag(LayerAndTag::Tags::Occupied);
	gameObject->GetComponent<RenderObject*>()->GetMaterial()->SetColour(obj->GetComponent<RenderObject*>()->GetMaterial()->GetColour());
	SetTarget(obj);
}

void Resource::Reset() 
{
	gameObject->SetTag(LayerAndTag::Tags::Resources);
	gameObject->GameObject::SetParent(GameObject::FindGameObjectWithTag(LayerAndTag::Tags::ResourceParent));
	gameObject->GetComponent<RenderObject*>()->GetMaterial()->SetColour(Vector4(1,1,1,1));
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

