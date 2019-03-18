#include "HealthManager.h"



HealthManager::HealthManager(GameObject * obj) : ScriptObject(obj)
{
	health = 0;
}

HealthManager::HealthManager(GameObject * obj, int hp) : ScriptObject(obj)
{
	health = hp;
	maxHealth = hp;
}


HealthManager::~HealthManager()
{
}

void HealthManager::SetHealth(int h)
{
	maxHealth = h;
	health = h;
}

int HealthManager::GetHealth() const
{
	return health;
}

int HealthManager::GetMaxHealth() const
{
	return maxHealth;
}

void HealthManager::TakeDamage(int amount)
{
	health -= amount;

	if (health <= 0) { health = 0; }
}

bool HealthManager::IsDead()
{
	return (health <= 0);
}


