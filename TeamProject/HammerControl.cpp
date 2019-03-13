#include "HammerControl.h"
#include "Resource.h"


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
		if ((int)handle->GetTransform().GetChildrenList().size() > 0)
		{
			for (auto&i : children)
			{
				i->GetTransform().SetParent(handle->GetTransform().GetChildrenList()[0]);
				i->GetTransform().ForceUpdateLocalPositionWithTransform(GenerateRandomPositionInHammer());
				i->GetComponent<DamageControl*>()->SetDamage(1);
				i->GetComponent<DamageControl*>()->SetTypeOfDamage(DamageControl::DamageType::Continuous);
			}	
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

	temp.x = (rand() % (int)(maxX - minX)) + minX;
	temp.y = (rand() % (int)(maxY - minY)) + minY;
	temp.z = (rand() % (int)(maxZ - minZ)) + minZ;

	return temp;
}
