#pragma once
#include "ScriptObject.h"

class HammerControl: public ScriptObject
{
public:
	HammerControl(GameObject * gameObject);
	~HammerControl();
	void SetHandle(GameObject * h);
	void ActivateHammer();
	void DeactivateHammer();
	void HammerHit();
	int GetHitCounter() const;
	void SetHammerSize(float minx, float maxx, float miny, float maxy, float minz, float maxz);
	void Awake() override;

protected:
	GameObject * handle = nullptr;
	int hitCounter = 0;

	float minX;
	float minY;
	float minZ;

	float maxX;
	float maxY;
	float maxZ;

	Vector3 GenerateRandomPositionInHammer();
};

