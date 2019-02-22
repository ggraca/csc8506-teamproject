#include "HealthManager.h"



HealthManager::HealthManager(GameObject * obj) : ScriptObject(obj)
{
	health = 0;
}


HealthManager::~HealthManager()
{
}

void HealthManager::SetHealth(int h)
{
	health = h;
}

int HealthManager::GetHealth() const
{
	return health;
}

void HealthManager::TakeDamage(int amount)
{
	health -= amount;

	if (health < 0) { health = 0; }
}

bool HealthManager::IsDead()
{
	return (health <= 0);
}


