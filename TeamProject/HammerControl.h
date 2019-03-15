#pragma once
#include "ScriptObject.h"

class HammerControl: public ScriptObject
{
public:
	HammerControl(GameObject * gameObject);
	~HammerControl();
	void SetHandle(GameObject * h);
	void SetHandleCollision(GameObject * hc);
	void ActivateHammer();
	void FormHammer();
	void DeformHammer();
	void DeactivateHammer();
	void HammerHit();
	int GetHitCounter() const;
	void SetHammerSize(float minx, float maxx, float miny, float maxy, float minz, float maxz);
	void Awake() override;

protected:
	GameObject * handle = nullptr;
	GameObject * handleCollision = nullptr;
	int hitCounter = 0;

	float minX;
	float minY;
	float minZ;

	float maxX;
	float maxY;
	float maxZ;

	Vector3 GenerateRandomPositionInHammer();
};

