#include "HammerControl.h"
#include "Resource.h"
#include "PhysicsObject.h"
#include "GameWorld.h"
#include "BulletPhysics.h"
HammerControl::HammerControl(GameObject * gameObject):ScriptObject(gameObject)
{
}

HammerControl::~HammerControl()
{
}

void HammerControl::Awake()
{
	DeactivateHammer();
}

void HammerControl::SetHandle(GameObject * h)
{
	handle = h;
}

void HammerControl::SetHandleCollision(GameObject * hc)
{
	handleCollision = hc;
}

void HammerControl::ActivateHammer()
{
	if (!handle || !handleCollision) { return; }

	handle->SetActiveStatus(true);

	FormHammer();
	handleCollision->GetComponent<DamageControl*>()->SetDamage((int)handle->GetTransform().GetChildrenList().size() - 1);
	
	handleCollision->AddComponent<PhysicsObject*>(new PhysicsObject(&handleCollision->GetTransform(), ShapeType::cube, 0));
	GameObject::gameWorld->AddObjectPhysicsToWorld(handleCollision->GetComponent<PhysicsObject*>());
}

void HammerControl::FormHammer()
{
	//Will be changed later on
	auto children = GameObject::FindGameObjectsWithTag(LayerAndTag::Tags::Occupied);

	for (auto&i : children)
	{
		i->GetComponent<Resource*>()->SetTarget(nullptr);
		i->RemoveComponent<PhysicsObject*>();
		GameObject::gameWorld->RemoveCollisionsFromGameObject(i);
		i->GetTransform().SetLocalScale(Vector3(5,5,5)/Vector3(2.0f,25.0f,2.0f));
		i->GetTransform().SetLocalPosition(GenerateRandomPositionInHammer());
		i->SetParent(handle);
	}
}

void HammerControl::DeformHammer()
{
	//Will be changed later on
	auto children = handle->GetTransform().GetChildrenList();
	
	for (auto&i : children)
	{
		if (i->GetGameObject()->GetTag() == LayerAndTag::Tags::HammerHead) { continue; }

		i->GetGameObject()->GetComponent<Resource*>()->SetTarget(gameObject);
		i->GetGameObject()->SetParent(GameObject::FindGameObjectWithTag(LayerAndTag::Tags::CaptureParent));
		i->GetGameObject()->GetTransform().SetLocalScale(Vector3(5,5,5));
		i->GetGameObject()->AddComponent<PhysicsObject*>(new PhysicsObject(i, ShapeType::cube, 10));

		GameObject::gameWorld->AddObjectPhysicsToWorld(i->GetGameObject()->GetComponent<PhysicsObject*>());
	}
}

void HammerControl::DeactivateHammer()
{
	if (!handle) { return; }
	
	handle->SetActiveStatus(false);

	DeformHammer();

	handleCollision->RemoveComponent<PhysicsObject*>();
	GameObject::gameWorld->RemoveCollisionsFromGameObject(handleCollision);
	//handleCollision->GetComponent<DamageControl*>()->SetDamage(0);
}

void HammerControl::HammerHit()
{
	if (!handle) { return; }

	hitCounter = (hitCounter + 1) % 3;
}

int HammerControl::GetHitCounter() const
{
	return hitCounter;
}

void HammerControl::SetHammerSize(float minx, float maxx, float miny, float maxy, float minz, float maxz)
{
	minX = minx;
	maxX = maxx;

	minY = miny;
	maxY = maxy;

	minZ = minz;
	maxZ = maxz;
}


Vector3 HammerControl::GenerateRandomPositionInHammer()
{
	Vector3 temp;

	temp.x = ((float)(rand() % (int)(maxX - minX))) / 1000 + (minX/1000.0f);
	temp.y = ((float)(rand() % (int)(maxY - minY))) / 1000 + (minY/1000.0f);
	temp.z = ((float)(rand() % (int)(maxZ - minZ))) / 1000 + (minZ/1000.0f);

	return temp;
}
