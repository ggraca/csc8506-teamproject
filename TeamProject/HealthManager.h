#pragma once
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;

class HealthManager : virtual public ScriptObject
{
public:
	HealthManager(GameObject* obj);
	virtual ~HealthManager();

	void SetHealth(int h);
	int GetHealth() const;
	void TakeDamage(int amount);
	bool IsDead();

private:
	int health;
};

