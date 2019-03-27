#include "GunControl.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "AudioEngine.h"
#include "GameWorld.h"

#include "Debug.h"
GunControl::GunControl()
{
}

GunControl::GunControl(GameObject * gameObject):ScriptObject(gameObject)
{
}

GunControl::GunControl(GameObject * gameObject, float projSpeed, int projDamage) : ScriptObject(gameObject)
{
	projectileSpeed = projSpeed;
	idealProjectileDamage = projDamage;
}

GunControl::~GunControl()
{
}

void GunControl::Awake()
{
	camera = GameObject::GetMainCamera();
	DeactivateGun();
}

void GunControl::Update(float dt)
{
}

void GunControl::SetLeftGun(GameObject * obj)
{
	leftGun = obj;
}

void GunControl::SetRightGun(GameObject * obj)
{
	rightGun = obj;
}

GameObject * GunControl::GetLeftGun()
{
	return leftGun;
}

GameObject * GunControl::GetRightGun()
{
	return rightGun;
}

Vector3 GunControl::CalculateDirection()
{
	Vector3 forward;
	Vector3 ctransform = GetCameraRotation().ToEuler();
	ctransform.y *= -1;

	forward.x = sin(ctransform.y* (PI / 180)) * cos(ctransform.x * (PI / 180));
	forward.y = sin(ctransform.x * (PI / 180));
	forward.z = -1 * cos(ctransform.x * (PI / 180)) * cos(ctransform.y * (PI / 180));

	return forward.Normalised();
}

void GunControl::ActivateGun()
{
	if (!leftGun || !rightGun) { return; }

	leftGun->SetActiveStatus(true);
	rightGun->SetActiveStatus(true);
}

void GunControl::DeactivateGun()
{
	if (!leftGun || !rightGun) { return; }

	leftGun->SetActiveStatus(false);
	rightGun->SetActiveStatus(false);
}

void GunControl::FireObjectAndRemoveFromResources(std::vector<GameObject *> &children, int i )
{
	children[i]->GetComponent<Resource*>()->Reset();
	children[i]->GetComponent<DamageControl*>()->SetDamage(idealProjectileDamage);

	Vector3 projMov = (GetCameraPosition() + (CalculateDirection() * projectileSpeed)) - children[i]->GetTransform().GetWorldPosition();
	children[i]->GetComponent<PhysicsObject*>()->SetLinearVelocity(projMov);
	gameObject->GetComponent<Player*>()->LoseResource(children[i]);
}

void GunControl::FireObjectAndRemoveFromResources(GameObject * child)
{
	child->GetComponent<Resource*>()->Reset();
	child->GetComponent<DamageControl*>()->SetDamage(idealProjectileDamage);

	Vector3 projMov = (GetCameraPosition() + (CalculateDirection() * projectileSpeed)) - child->GetTransform().GetWorldPosition();
	child->GetComponent<PhysicsObject*>()->SetLinearVelocity(projMov);
	gameObject->GetComponent<Player*>()->LoseResource(child);
}

void GunControl::Fire()
{
	GameObject::gameWorld->GetAudio()->PlayEvent("event:/small-gun", gameObject->GetTransform().GetWorldPosition());
	if (!leftGun || !rightGun) { return; }

	auto child = gameObject->GetComponent<Player*>()->GetResources()[0];

	if (child)
	{
		child->GetTransform().ForceUpdateLocalPositionWithTransform(gameObject->GetTransform().GetChildrenList()[currentGun=!currentGun]->GetWorldPosition() + CalculateDirection() *30.0f );
		FireObjectAndRemoveFromResources(child);
	}
}


