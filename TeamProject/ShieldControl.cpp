#include "ShieldControl.h"
#include "PhysicsObject.h"
#include "GameWorld.h"

ShieldControl::ShieldControl(GameObject * obj) : ScriptObject(obj)
{
}


ShieldControl::~ShieldControl()
{
}

void ShieldControl::Awake()
{
	DeactivateShield();
}

void ShieldControl::SetTarget(Transform * t)
{
	target = t;
}

void ShieldControl::Update(float dt)
{
	FollowTarget();
}

void ShieldControl::ActivateShield()
{
	if (!shield) { return; }

	shield->SetActiveStatus(true);

	shield->AddComponent<PhysicsObject*>(new PhysicsObject(&shield->GetTransform(), ShapeType::cube, 0));
	GameObject::gameWorld->AddObjectPhysicsToWorld(shield->GetComponent<PhysicsObject*>());
}

void ShieldControl::DeactivateShield()
{
	if (!shield) { return; }

	shield->SetActiveStatus(false);

	shield->RemoveComponent<PhysicsObject*>();
	GameObject::gameWorld->RemoveCollisionsFromGameObject(shield);
}

void ShieldControl::SetShield(GameObject * s)
{
	shield = s;
}

void ShieldControl::FollowTarget()
{
	if (shield->IsActive() &&(!shield || !target)) { return; }

	shield->GetTransform().ForceUpdateWorldPosition(target->GetWorldPosition());
	shield->GetTransform().SetLocalOrientation(target->GetGameObject()->GetTransform().GetWorldOrientation());
}

void ShieldControl::FormShield()
{

}

void ShieldControl::DeformShield()
{

}