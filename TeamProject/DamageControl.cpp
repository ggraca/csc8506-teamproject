#include "DamageControl.h"



DamageControl::DamageControl(GameObject * obj): ScriptObject(obj)
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
	if (!otherObject) { return; }

	ResolveDamage(otherObject);
}

void DamageControl::ResolveDamage(GameObject * obj)
{
	if (damage == 0) { return; }

	if (gameObject->CompareTag(LayerAndTag::Tags::EnemyProjectile) && obj->CompareTag(LayerAndTag::Tags::Player)) 
	{
		obj->GetComponent<Player*>()->UpdateResourceCount(-damage);
		
		if (typeOfDamage == DamageType::SingleShot) { damage = 0; }
	}

	else if (gameObject->CompareTag(LayerAndTag::Tags::EnemyProjectile)) 
	{
		if (typeOfDamage == DamageType::SingleShot) { damage = 0; }
	}

	else if (gameObject->CompareTag(LayerAndTag::Tags::Occupied) || gameObject->CompareTag(LayerAndTag::Tags::Resources))
	{
		if (obj->GetComponent<HealthManager*>())
		{
			obj->GetComponent<HealthManager*>()->TakeDamage(damage);
		}

		if (typeOfDamage == DamageType::SingleShot) { damage = 0; }
	}
}

void DamageControl::ResetDamageControl()
{
	damage = 0;
	typeOfDamage = DamageType::SingleShot;
}

void DamageControl::SetDamage(int d)
{
	damage = d;
}



