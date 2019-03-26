#include "HammerControl.h"
#include "Resource.h"
#include "PhysicsObject.h"
#include "GameWorld.h"
#include "BulletPhysics.h"
#include "Debug.h"

HammerControl::HammerControl(GameObject * gameObject):ScriptObject(gameObject)
{
}

HammerControl::~HammerControl()
{
}

void HammerControl::Awake()
{
	DeactivateHammer();
}

void HammerControl::SetHandle(GameObject * h)
{
	handle = h;
}

Vector3 HammerControl::CalculateDirection()
{
	Vector3 forward;
	Vector3 ctransform = GetCameraRotation().ToEuler();
	ctransform.y *= -1;

	forward.x = sin(ctransform.y* (PI / 180)) * cos(ctransform.x * (PI / 180));
	forward.y = sin(ctransform.x * (PI / 180));
	forward.z = -1 * cos(ctransform.x * (PI / 180)) * cos(ctransform.y * (PI / 180));

	return forward.Normalised();
}

void HammerControl::ActivateHammer()
{
	if (!handle) { return; }

	handle->SetActiveStatus(true);

	FormHammer();
}

void HammerControl::FormHammer()
{
	//Will be changed later on
	auto children = gameObject->GetComponent<Player*>()->GetResources();

	for (auto&i : children)
	{
		i->GetComponent<Resource*>()->SetTarget(nullptr);
		GameObject::gameWorld->RemoveCollisionsFromGameObject(i);
		i->RemoveComponent<PhysicsObject*>();
		i->GetTransform().ForceUpdateScaleWithTransform(Vector3(2.5,2.5/12.5,2.5));
		i->GetTransform().SetLocalPosition(GenerateRandomPositionInHammer());
		i->SetParent(handle);
	}
}

void HammerControl::DeformHammer()
{
	//Will be changed later on
	auto children = handle->GetTransform().GetChildrenList();
	
	for (auto&i : children)
	{
		if (i->GetGameObject()->GetTag() == LayerAndTag::Tags::HammerHead) { continue; }

		Vector3 currentPos = i->GetWorldPosition();
		i->GetGameObject()->GetComponent<Resource*>()->SetTarget(gameObject);
		i->GetGameObject()->SetParent(nullptr);
		i->SetLocalScale(Vector3(5,5,5));
		i->GetGameObject()->AddComponent<PhysicsObject*>(new PhysicsObject(i, ShapeType::cube, 10));
		i->SetWorldPosition(currentPos);

		GameObject::gameWorld->AddObjectPhysicsToWorld(i->GetGameObject()->GetComponent<PhysicsObject*>());
	}
}

void HammerControl::DeactivateHammer()
{
	if (!handle) { return; }
	
	handle->SetActiveStatus(false);

	DeformHammer();
}

void HammerControl::HammerHit()
{
	if (!handle) { return; }

	//auto camera = GameObject::gameWorld->GetMainCamera();
	Vector3 end;
	GameObject * colliding = GameObject::gameWorld->CollisionObjectToGameObject(GameObject::gameWorld->Raycast(gameObject->GetTransform().GetWorldPosition(), gameObject->GetTransform().GetWorldPosition() + (CalculateDirection() * 100), end));
	if (colliding)
	{		
		if (colliding->GetComponent<HealthManager*>())
		{
			colliding->GetComponent<HealthManager*>()->TakeDamage((int)handle->GetTransform().GetChildrenList().size());
		}
	}

	hitCounter = (hitCounter + 1) % 3;
}

int HammerControl::GetHitCounter() const
{
	return hitCounter;
}

void HammerControl::SetHammerSize(float minx, float maxx, float miny, float maxy, float minz, float maxz)
{
	minX = minx;
	maxX = maxx;

	minY = miny;
	maxY = maxy;

	minZ = minz;
	maxZ = maxz;
}


Vector3 HammerControl::GenerateRandomPositionInHammer()
{
	Vector3 temp;

	temp.x = ((float)(rand() % (int)(maxX - minX))) / 1000 + (minX/1000.0f);
	temp.y = ((float)(rand() % (int)(maxY - minY))) / 1000 + (minY/1000.0f);
	temp.z = ((float)(rand() % (int)(maxZ - minZ))) / 1000 + (minZ/1000.0f);

	return temp;
}
