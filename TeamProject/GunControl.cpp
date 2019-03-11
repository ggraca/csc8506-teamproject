#include "GunControl.h"
#include "PhysicsObject.h"

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
}

void GunControl::Update(float dt)
{
	camera = GameObject::GetMainCamera();
	Debug::DrawLine(gameObject->GetTransform().GetWorldPosition(), gameObject->GetTransform().GetWorldPosition() + (CalculateDirection() * 100));
}

Vector3 GunControl::CalculateDirection()
{
	if (!camera || camera->GetTransform().GetChildrenList().size() == 0) { return Vector3(0,0,0); }

	Vector3 forward;
	Transform *ctransform = camera->GetTransform().GetChildrenList()[0];

	forward.x = sin(camera->GetTransform().GetLocalOrientation().ToEuler().y * (PI / 180)) * cos(camera->GetTransform().GetLocalOrientation().ToEuler().x * (PI / 180));
	forward.y = sin(-camera->GetTransform().GetLocalOrientation().ToEuler().x * (PI / 180));
	forward.z = cos(camera->GetTransform().GetLocalOrientation().ToEuler().x * (PI / 180)) * cos(camera->GetTransform().GetLocalOrientation().ToEuler().y * (PI / 180));

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
	auto children = GameObject::FindGameObjectsWithTag(LayerAndTag::Tags::Occupied);

	if (children.size() > 0)
	{
		children[0]->GetTransform().ForceUpdateLocalPositionWithTransform(gameObject->GetTransform().GetWorldPosition()+  CalculateDirection() * 50);
		children[0]->GetComponent<Resource*>()->Reset();
		children[0]->GetComponent<DamageControl*>()->SetDamage(1);
		children[0]->GetComponent<Resource*>()->Reset();

		//Vector3 projMov = ((camera->GetTransform().GetChildrenList()[0]->GetWorldPosition() + (CalculateDirection()  * projectileSpeed)) - children[0]->GetTransform().GetWorldPosition()).Normalised() * projectileSpeed;
		Vector3 projMov = CalculateDirection() * projectileSpeed;
		projMov.y -=(camera->GetTransform().GetChildrenList()[0]->GetWorldPosition().y - gameObject->GetTransform().GetWorldPosition().y);
		children[0]->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(btVector3(projMov.x,projMov.y,projMov.z));
		gameObject->GetComponent<Player*>()->UpdateResourceCount(-1);
		
	}
}


