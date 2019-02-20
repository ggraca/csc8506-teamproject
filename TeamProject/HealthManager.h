#pragma once
#include "GameObject.h"

class HealthManager
{
public:
	HealthManager();
	~HealthManager();

	void SetHealth(int h);
	int GetHealth();
	void TakeDamage(int amount);

private:
	int health;
};

