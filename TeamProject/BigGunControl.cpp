#include "BigGunControl.h"
#include "PhysicsObject.h"
#include "AudioEngine.h"
#include "GameWorld.h"


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

	int projectileAmount = 3 + (int)(time / 0.3f);

	if (projectileAmount >= gameObject->GetComponent<Player*>()->GetResourceCount())
	{
		projectileAmount = gameObject->GetComponent<Player*>()->GetResourceCount();
	}

	auto children = gameObject->GetComponent<Player*>()->GetResources();

	for (int i = 0; i < projectileAmount; i++)
	{
		children[i]->GetTransform().ForceUpdateLocalPositionWithTransform(bigGun->GetTransform().GetWorldPosition() + CalculateDirection() *80.0f);
		FireObjectAndRemoveFromResources(children,i);
	}

	GameObject::gameWorld->GetAudio()->PlayEvent("event:/shotgun", bigGun->GetTransform().GetWorldPosition());
}

void BigGunControl::SetBigGun(GameObject * gun)
{
	bigGun = gun;
}

GameObject * BigGunControl::GetBigGun()
{
	return bigGun;
}
