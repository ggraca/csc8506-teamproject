#pragma once
#include "ScriptObject.h"
#define PI 3.14159265358979323846f

class HammerControl: public ScriptObject
{
public:
	HammerControl(GameObject * gameObject);
	~HammerControl();
	void SetHandle(GameObject * h);
	Vec3 CalculateDirection();
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
	int hitCounter = 0;

	float minX;
	float minY;
	float minZ;

	float maxX;
	float maxY;
	float maxZ;

	Vec3 GenerateRandomPositionInHammer();
};

