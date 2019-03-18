#pragma once
#include "ScriptObject.h"

using namespace NCL;
using namespace CSC8503;

class HealthManager : virtual public ScriptObject
{
public:
	HealthManager(GameObject* obj);
	HealthManager(GameObject* obj,int hp);
	virtual ~HealthManager();

	void SetHealth(int h);
	int GetHealth() const;
	int GetMaxHealth() const;
	void TakeDamage(int amount);
	bool IsDead();

private:
	int health;
	int maxHealth;
};

