#pragma once
#include "GameObject.h"
#include "HealthManager.h"
#include "Player.h"

using namespace NCL;
using namespace CSC8503;

class DamageControl : virtual public ScriptObject
{
public:
	DamageControl(GameObject * obj);
	~DamageControl();

	static enum DamageType 
	{
		SingleShot,
		Continuous
	};

	void SetTypeOfDamage(DamageType d);
	DamageType GetTypeOfDamage(DamageType typeOfDamage);
	void OnCollisionBegin(GameObject * otherObject);
	void ResolveDamage(GameObject * obj);
	void ResetDamageControl();

private:
	int damage = 0;
	DamageType typeOfDamage = DamageType::SingleShot;
	
};

