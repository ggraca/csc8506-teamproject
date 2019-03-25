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

	virtual void ActivateGun();
	virtual void DeactivateGun();
	void FireObjectAndRemoveFromResources(std::vector<GameObject *> &children, int i=0);
	void FireObjectAndRemoveFromResources(GameObject * child);
	void Fire();

	void Awake()  override;
	void Update(float dt) override;

	void SetLeftGun(GameObject * obj);
	void SetRightGun(GameObject * obj);

protected:

	float projectileSpeed = 6000.0f;
	int idealProjectileDamage = 4;
	GameObject * camera = nullptr;
	GameObject * leftGun = nullptr;
	GameObject * rightGun = nullptr;

	Vector3 CalculateDirection();
	bool currentGun = false;
};

