#include "GunControl.h"
#include "PhysicsObject.h"
#include "RenderObject.h"

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

Vector3 GunControl::CalculateDirection()
{
	if (!camera || camera->GetTransform().GetChildrenList().size() == 0) { return Vector3(0,0,0); }

	Vector3 forward;
	Transform *ctransform = camera->GetTransform().GetChildrenList()[0];

	forward.x = sin(camera->GetTransform().GetLocalOrientation().ToEuler().y* (PI / 180)) * cos(camera->GetTransform().GetLocalOrientation().ToEuler().x * (PI / 180));
	forward.y = sin(-camera->GetTransform().GetLocalOrientation().ToEuler().x * (PI / 180));
	forward.z = cos(camera->GetTransform().GetLocalOrientation().ToEuler().x * (PI / 180)) * cos(camera->GetTransform().GetLocalOrientation().ToEuler().y * (PI / 180));

	return forward.Normalised();
}

void GunControl::ActivateGun()
{
	//Animation
	leftGun->SetActiveStatus(true);
	rightGun->SetActiveStatus(true);
}

void GunControl::DeactivateGun()
{
	//Animation
	leftGun->SetActiveStatus(false);
	rightGun->SetActiveStatus(false);
}

void GunControl::Fire()
{
	//This part will change later on
	auto children = GameObject::FindGameObjectsWithTag(LayerAndTag::Tags::Occupied);

	if (children.size() > 0)
	{
		children[0]->GetTransform().ForceUpdateLocalPositionWithTransform(gameObject->GetTransform().GetChildrenList()[currentGun=!currentGun]->GetWorldPosition() + CalculateDirection() *30.0f );
		children[0]->GetComponent<Resource*>()->Reset();
		children[0]->GetComponent<DamageControl*>()->SetDamage(4);

		Vector3 projMov = CalculateDirection() * projectileSpeed;
		projMov.y = (camera->GetTransform().GetChildrenList()[0]->GetWorldPosition()).y + 60;
		children[0]->GetComponent<PhysicsObject*>()->SetLinearVelocity(projMov);
		gameObject->GetComponent<Player*>()->UpdateResourceCount(-1);
		
	}
}

