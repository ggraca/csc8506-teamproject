#include "AirStrikeControl.h"
#include "GameWorld.h"
#include "Debug.h"
#include "Resource.h"
#include "PhysicsObject.h"

AirStrikeControl::AirStrikeControl(GameObject * obj) : ScriptObject(obj)
{
	airStrikeDistance = 5000.0f;
}


AirStrikeControl::~AirStrikeControl()
{
}

void AirStrikeControl::Awake()
{
}

void AirStrikeControl::Update(float dt)
{
	Debug::DrawLine(gameObject->GetTransform().GetWorldPosition(), GameObject::gameWorld->GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition() + (CalculateDirection() * 500));
}

void AirStrikeControl::LaunchAirStrike()
{
	Vec3 forward = CalculateDirection();
	Vec3 start = GetCameraPosition();
	Vec3 end = start + ( forward * airStrikeDistance);
	Vec3 hit;
	auto res = GameObject::gameWorld->Raycast(start, end, hit);

	auto children = GameObject::FindGameObjectsWithTag(gameObject->GetComponent<Player*>()->GetResourceTag());

	if ((int)children.size() < 5) { return; }

	if (res)
	{
		for (int i = 0; i < 5; i++)
		{
			children[i]->GetComponent<Resource*>()->Reset();
			children[i]->GetTransform().SetWorldPosition(hit + Vec3(0, 500, 0) + (forward  * 30));
			children[i]->GetComponent<PhysicsObject*>()->SetLinearVelocity(Vec3(0, -400.0f, 0));
			children[i]->GetComponent<DamageControl*>()->SetDamage(1);
			gameObject->GetComponent<Player*>()->UpdateResourceCount(-1);
			
		}
	}
}

Vec3 AirStrikeControl::CalculateDirection()
{
	Vec3 forward;
	Vec3 ctransform = GetCameraRotation().ToEuler();
	ctransform.y *= -1;

	forward.x = sin(ctransform.y* (PI / 180)) * cos(ctransform.x * (PI / 180));
	forward.y = sin(ctransform.x * (PI / 180));
	forward.z = -1*cos(ctransform.x * (PI / 180)) * cos(ctransform.y * (PI / 180));

	return forward.Normalised();
}