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

void DamageControl::Update(float dt)
{
	FollowTarget();
}

void DamageControl::SetTypeOfDamage(DamageType d)
{
	this->typeOfDamage = d;
}


DamageControl::DamageType DamageControl::GetTypeOfDamage(DamageControl::DamageType typeOfDamage)
{
	return typeOfDamage;
}

void DamageControl::SetWeaponType(WeaponType w)
{
	typeOfWeapon = w;
}

void DamageControl::ResolveDamage(GameObject * obj)
{
	std::cout << "Colliding with " << obj->GetName() <<" as "<<gameObject->GetName() <<" amount: " << damage << std::endl;

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

	else if (gameObject->CompareTag(LayerAndTag::Tags::Occupied) || gameObject->CompareTag(LayerAndTag::Tags::Resources) || gameObject->CompareTag(LayerAndTag::Tags::HammerHead))
	{
		if (obj->GetComponent<HealthManager*>())
		{
			obj->GetComponent<HealthManager*>()->TakeDamage(damage);
			std::cout << "Dealt damage to " << obj->GetName() << " amount: " << damage<<std::endl;
		}

		if (typeOfDamage == DamageType::SingleShot) { damage = 0; }
	}
}

void DamageControl::ResetDamageControl()
{
	damage = 0;
	typeOfDamage = DamageType::SingleShot;
	typeOfWeapon = WeaponType::Regular;
	target = nullptr;
}

void DamageControl::SetDamage(int d)
{
	damage = d;
}

void DamageControl::SetTarget(Transform * t)
{
	target = t;
}

Transform * DamageControl::GetTransform()
{
	return target;
}

void DamageControl::FollowTarget()
{
	if (target && typeOfWeapon == WeaponType::Dummy)
	{
		gameObject->GetTransform().ForceUpdateWorldPosition(target->GetWorldPosition());
	}
}



