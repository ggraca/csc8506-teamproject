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

}

void DamageControl::ResolveDamage()
{
	if (damage == 0) { return; }

	if(GameObject::FindGameObjectWithTag(LayerAndTag::Tags::EnemyProjectile) && GameObject::FindGameObjectWithTag(LayerAndTag::Tags::Player))
}




