#include "DamageControl.h"
#include "PhysicsObject.h"


DamageControl::DamageControl(GameObject * obj): ScriptObject(obj)
{
	ResetDamageControl();
}


DamageControl::~DamageControl()
{
}

void DamageControl::OnCollisionBegin(GameObject * otherObject)
{
	if (!otherObject) { return; }

	ResolveDamage(otherObject);
}

void DamageControl::SetTypeOfDamage(DamageType d)
{
	this->typeOfDamage = d;
}


DamageControl::DamageType DamageControl::GetTypeOfDamage(DamageControl::DamageType typeOfDamage)
{
	return typeOfDamage;
}


void DamageControl::ResolveDamage(GameObject * obj)
{
	if (damage == 0) { return; }

 	if (IsTagOccupied(gameObject->GetTag()) && obj->CompareTag(LayerAndTag::Tags::Player)) 
	{
		if (!gameObject->CompareTag(obj->GetComponent<Player*>()->GetResourceTag()))
		{
			obj->GetComponent<Player*>()->TakeDamage(damage);
			
			if (typeOfDamage == DamageType::SingleShot) { damage = 0; }
			gameObject->SetTag(LayerAndTag::Resources);
		}
	}

	else if (gameObject->CompareTag(LayerAndTag::Tags::EnemyProjectile) && obj->CompareTag(LayerAndTag::Tags::Player))
	{
		obj->GetComponent<Player*>()->TakeDamage(damage);

		if (typeOfDamage == DamageType::SingleShot) { damage = 0; }
		gameObject->SetTag(LayerAndTag::Resources);
	}

	else if (gameObject->CompareTag(LayerAndTag::Tags::EnemyProjectile)) 
	{
		if (typeOfDamage == DamageType::SingleShot) { damage = 0; }
		gameObject->SetTag(LayerAndTag::Resources);
	}

	else if ((IsTagOccupied(gameObject->GetTag()) || gameObject->CompareTag(LayerAndTag::Tags::Resources)) && !IsTagOccupied(obj->GetTag()) &&  !obj->CompareTag(LayerAndTag::Tags::Resources))
	{
		if (obj->GetComponent<HealthManager*>())
		{
			obj->GetComponent<HealthManager*>()->TakeDamage(damage);
		}

		if (typeOfDamage == DamageType::SingleShot) { damage = 0; }
		gameObject->SetTag(LayerAndTag::Resources);
	}
}

bool DamageControl::IsTagOccupied(LayerAndTag::Tags t)
{
	switch (t)
	{
	case LayerAndTag::Occupied0:
		return true;
	case LayerAndTag::Occupied1:
		return true;
	case LayerAndTag::Occupied2:
		return true;
	case LayerAndTag::Occupied3:
		return true;
	default:
		return false;
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





