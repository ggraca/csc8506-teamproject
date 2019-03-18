#include "ShieldControl.h"



ShieldControl::ShieldControl(GameObject * obj) : ScriptObject(obj)
{
}


ShieldControl::~ShieldControl()
{
}

void ShieldControl::SetTarget(Transform * t)
{
	target = t;
}

void ShieldControl::Update(float dt)
{
}

void ShieldControl::ActivateShield()
{
}

void ShieldControl::DeactivateShield()
{
}
