#include "GunControl.h"
#include "PhysicsObject.h"


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
}

Vector3 GunControl::CalculateDirection()
{
	if (!camera || camera->GetTransform().GetChildrenList().size() == 0) { return Vector3(0,0,0); }

	Vector3 forward;
	Transform *ctransform = camera->GetTransform().GetChildrenList()[0];

	forward.x = sin(ctransform->GetLocalOrientation().ToEuler().y * (PI / 180)) * cos(ctransform->GetLocalOrientation().ToEuler().x * (PI / 180));
	forward.y = sin(-ctransform->GetLocalOrientation().ToEuler().x * (PI / 180));
	forward.z = cos(ctransform->GetLocalOrientation().ToEuler().x * (PI / 180)) * cos(ctransform->GetLocalOrientation().ToEuler().y * (PI / 180));

	return forward.Normalised();
}

void GunControl::ActivateGun()
{
	//Animation
}

void GunControl::DeactivateGun()
{
	//Animation
}

void GunControl::Fire()
{
	//This part will change later on
	auto children = GameObject::GetChildrenOfObject(GameObject::FindGameObjectWithTag(LayerAndTag::Tags::Player));

	if (children.size() > 0)
	{
		children[0]->GetComponent<Resource*>()->Reset();
		children[0]->GetComponent<DamageControl*>()->SetDamage(1);
		children[0]->GetComponent<PhysicsObject*>()->GetRigidbody()->addConstraintRef()
		gameObject->GetComponent<Player*>()->UpdateResourceCount(-1);
		
	}
}


