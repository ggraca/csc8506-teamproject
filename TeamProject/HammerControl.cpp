#include "HammerControl.h"
#include "Resource.h"
#include "PhysicsObject.h"

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

void HammerControl::ActivateHammer()
{
	if (!handle) { return; }

	handle->SetActiveStatus(true);

	//Will be changed later on
	auto children = GameObject::FindGameObjectsWithTag(LayerAndTag::Tags::Occupied);

	if ((int)children.size() > 0)
	{
		for (auto&i : children)
		{
			i->SetParent(handle);
			handle->GetTransform().UpdateMatrices();
			i->GetTransform().SetLocalPosition(Vector3(10,10,10));
			i->GetTransform().SetLocalScale(Vector3(10,10,10)/*(i->GetTransform().GetLocalScale()/handle->GetTransform().GetLocalScale())/gameObject->GetTransform().GetLocalScale()*/);

			i->GetComponent<Resource*>()->SetTarget(nullptr);
			i->GetComponent<DamageControl*>()->SetDamage(1);
			i->GetComponent<DamageControl*>()->SetTypeOfDamage(DamageControl::DamageType::Continuous);
		}	
	}
}

void HammerControl::DeactivateHammer()
{
	if (!handle) { return; }
	
	handle->SetActiveStatus(false);
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

	temp.x = ((float)(rand() % (int)(maxX - minX))) / 1000 + minX;
	temp.y = ((float)(rand() % (int)(maxY - minY))) / 1000 + minY;
	temp.z = ((float)(rand() % (int)(maxZ - minZ))) / 1000 + minZ;

	return temp;
}
