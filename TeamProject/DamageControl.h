#pragma once
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;

class DamageControl
{
public:
	DamageControl();
	~DamageControl();

	static enum DamageType 
	{
		SingleShot,
		Continuous
	};

	void SetTypeOfDamage(DamageType d);
	DamageType GetTypeOfDamage(DamageType typeOfDamage);
	void OnCollisionBegin(GameObject * otherObject);
	void ResolveDamage();

private:
	int damage = 0;
	DamageType typeOfDamage = DamageType::SingleShot;
	
};

