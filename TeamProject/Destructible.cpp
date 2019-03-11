#include "Destructible.h"
#include "InputManager.h"

Destructible::Destructible(GameObject * gameObject):ScriptObject(gameObject)
{
	resourceDimensions = Vector3(5, 5, 5);
}

Destructible::~Destructible()
{
}


void Destructible::Awake()
{
}

void Destructible::Start()
{

}

void Destructible::Update(float dt)
{
	CheckIfDestroyed();
}

void Destructible::CheckIfDestroyed()
{
	HealthManager * health = gameObject->GetComponent<HealthManager*>();
	if (health->IsDead())
	{
		for (int i = 0; i < (health->GetHealth()/25);i++)
		{
			GenerateResource();
		}
		GameObject::gameWorld->LateDestroy(gameObject);
	}
}

void Destructible::LateUpdate(float dt)
{
}

void Destructible::OnCollisionBegin(GameObject * otherObject)
{
}


void Destructible::OnCollisionEnd(GameObject * otherObject)
{
}

void Destructible::GenerateResource()
{

	Vector3 destPosition = gameObject->GetTransform().GetWorldPosition();
	Vector3 destScale = gameObject->GetTransform().GetLocalScale();

	float minX = destPosition.x - ((destScale.x/2) - (resourceDimensions.x / 2));
	float maxX = destPosition.x + ((destScale.x / 2) - (resourceDimensions.x / 2));

	float minY = destPosition.y+50 - ((destScale.y / 2) - (resourceDimensions.y / 2));
	float maxY = destPosition.y+50 + ((destScale.y / 2) - (resourceDimensions.y / 2));

	float minZ = destPosition.z - ((destScale.z / 2) - (resourceDimensions.z / 2));
	float maxZ = destPosition.z + ((destScale.z / 2) - (resourceDimensions.z / 2));

	float resourceX = (rand() % (int)(maxX - minX)) + minX;
	float resourceY = (rand() % (int)(maxY - minY)) + minY;
	float resourceZ = (rand() % (int)(maxZ - minZ)) + minZ;

	ResourcePrefab * resource = new ResourcePrefab(Vector3(resourceX, resourceY, resourceZ),Quaternion::AxisAngleToQuaternion(Vector3(0,0,0),0),Vector3(5,5,5),1000,0.8f,0.4f);
	resource->GetComponent<PhysicsObject*>()->SetLinearVelocity(Vector3(0, -100, 0));
	GameObject::gameWorld->LateInstantiate(resource);
}
