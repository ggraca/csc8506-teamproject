#include "DamageControl.h"



DamageControl::DamageControl()
{
}


DamageControl::~DamageControl()
{
}

void DamageControl::SetTypeOfDamage(DamageType d)
{
	this->typeOfDamage = d;
}


DamageControl::DamageType DamageControl::GetTypeOfDamage(DamageControl::DamageType typeOfDamage)
{
	return typeOfDamage;
}

void DamageControl::OnCollisionBegin(GameObject * otherObject)
{
	ResolveDamage(otherObject);
}

void DamageControl::ResolveDamage(GameObject * obj)
{
	if (damage == 0) { return; }

	if (GameObject::FindGameObjectWithTag(LayerAndTag::Tags::EnemyProjectile) && GameObject::FindGameObjectWithTag(LayerAndTag::Tags::Player)) 
	{
		//obj->GetScript<Player*>()->TakeDamage(damage);
	
		obj->GetScript<HealthManager*>()->TakeDamage(damage);
		if (typeOfDamage = DamageType::SingleShot) { damage = 0; }
	}

	else if (GameObject::FindGameObjectWithTag(LayerAndTag::Tags::EnemyProjectile)) 
	{
		if (typeOfDamage = DamageType::SingleShot) { damage = 0; }
	}

	else if (GameObject::FindGameObjectWithTag(LayerAndTag::Tags::Occupied) || GameObject::FindGameObjectWithTag(LayerAndTag::Tags::Resources) && GameObject::FindGameObjectWithTag(LayerAndTag::Tags::Enemy))
	{
		//obj->GetScript<Enemy*>()->TakeDamage(damage);
		if (typeOfDamage = DamageType::SingleShot) { damage = 0; }
	}

	else if (GameObject::FindGameObjectWithTag(LayerAndTag::Tags::Occupied) || GameObject::FindGameObjectWithTag(LayerAndTag::Tags::Resources) && GameObject::FindGameObjectWithTag(LayerAndTag::Tags::Destructable))
	{ 
		//obj->GetScript<Destructable*>()->TakeDamage(damage);
		if (typeOfDamage = DamageType::SingleShot) { damage = 0; }
	}
}



