#pragma once
#include "ScriptObject.h"
#include "Resource.h"

#define PI 3.14159265358979323846f

class GunControl : public ScriptObject
{
public:
	GunControl();
	GunControl(GameObject * gameObject);
	GunControl(GameObject * gameObject, float projSpeed, int projDamage);
	virtual ~GunControl();

	void ActivateGun();
	void DeactivateGun();
	void Fire();

	void Awake()  override;

protected:

	float projectileSpeed = 200.0f;
	int idealProjectileDamage = 4;
	GameObject * camera = nullptr;

	Vector3 CalculateDirection();
};

