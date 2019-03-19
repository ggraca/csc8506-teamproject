#include "BigGunControl.h"
#include "PhysicsObject.h"


BigGunControl::BigGunControl(GameObject * obj) : GunControl(obj)
{
}

BigGunControl::~BigGunControl()
{
}

void BigGunControl::ActivateGun()
{
	if (!bigGun) { return; }

	bigGun->SetActiveStatus(true);
}

void BigGunControl::DeactivateGun()
{
	if (!bigGun) { return; }

	bigGun->SetActiveStatus(false);
}

void BigGunControl::Fire(float time)
{
	if (!bigGun) { return; }

	int projectileAmount = (/*3 +*/ (int)(time / 0.5f)) % (gameObject->GetComponent<Player*>()->GetResourceCount()+1);
	std::cout << projectileAmount << std::endl;


	//This part will change later on
	auto children = GameObject::FindGameObjectsWithTag(LayerAndTag::Tags::Occupied);

	for (int i = 0; i < projectileAmount; i++)
	{
		children[i]->GetTransform().ForceUpdateLocalPositionWithTransform(bigGun->GetTransform().GetWorldPosition() + CalculateDirection() *200.0f);
		FireObjectAndRemoveFromResources(children,i);
	}
}

void BigGunControl::SetBigGun(GameObject * gun)
{
	bigGun = gun;
}
