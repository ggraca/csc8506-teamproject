#pragma once
#include "ScriptObject.h"
#define PI 3.14159265358979323846f

class AirStrikeControl : public ScriptObject
{

public:
	AirStrikeControl(GameObject * obj);
	~AirStrikeControl();

	void Awake() override;
	void Update(float dt) override;
	void LaunchAirStrike();

protected:
	Vec3 CalculateDirection();

	float airStrikeDistance = 0;
};

