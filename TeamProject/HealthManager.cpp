#include "HealthManager.h"



HealthManager::HealthManager()
{
}


HealthManager::~HealthManager()
{
}

void HealthManager::SetHealth(int h)
{
	health = h;
}

int HealthManager::GetHealth()
{
	return health;
}

void HealthManager::TakeDamage(int amount)
{
	health -= amount;
}


