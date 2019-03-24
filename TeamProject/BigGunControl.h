#pragma once
#include "GunControl.h"
class BigGunControl : public GunControl
{
public:

	BigGunControl(GameObject * obj);
	~BigGunControl();

	virtual void ActivateGun();
	virtual void DeactivateGun();
	void Fire(float time);

	void SetBigGun(GameObject * gun);

protected:
	GameObject * bigGun = nullptr;
};

