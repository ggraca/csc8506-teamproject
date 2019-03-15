#pragma once
#include "ScriptObject.h"
#include "HealthManager.h"
#include "Player.h"

using namespace NCL;
using namespace CSC8503;

class DamageControl : virtual public ScriptObject
{
public:
	DamageControl(GameObject * obj);
	~DamageControl();

	enum DamageType 
	{
		SingleShot,
		Continuous
	};

	enum WeaponType
	{
		Regular,
		Dummy
	};

	void SetTypeOfDamage(DamageType d);
	DamageType GetTypeOfDamage(DamageType typeOfDamage);
	void SetWeaponType(WeaponType w);
	void OnCollisionBegin(GameObject * otherObject);
	void ResolveDamage(GameObject * obj);
	void ResetDamageControl();
	void SetDamage(int d);
	void SetTarget(Transform * t);
	Transform* GetTransform();
	void Update(float dt) override;
	void FollowTarget();

protected:
	int damage = 0;
	DamageType typeOfDamage = DamageType::SingleShot;
	WeaponType typeOfWeapon = WeaponType::Regular;
	Transform * target = nullptr;
};

