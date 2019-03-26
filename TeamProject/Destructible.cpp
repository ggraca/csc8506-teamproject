#include "Destructible.h"
#include "InputManager.h"

Destructible::Destructible(GameObject * gameObject):ScriptObject(gameObject)
{
	resourceDimensions = Vec3(5, 5, 5);
}

Destructible::~Destructible()
{
}


void Destructible::Awake()
{
	health = gameObject->GetComponent<HealthManager*>();
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
	if (!health) { return; }

	if (health->IsDead())
	{
		for (int i = 0; i < (health->GetMaxHealth()/1);i++) //Will be changed to divide by 25
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

	Vec3 destPosition = gameObject->GetTransform().GetWorldPosition();
	Vec3 destScale = gameObject->GetTransform().GetLocalScale();

	float minX = destPosition.x - ((destScale.x/2) - (resourceDimensions.x / 2));
	float maxX = destPosition.x + ((destScale.x / 2) - (resourceDimensions.x / 2));

	float minY = destPosition.y+50 - ((destScale.y / 2) - (resourceDimensions.y / 2));
	float maxY = destPosition.y+50 + ((destScale.y / 2) - (resourceDimensions.y / 2));

	float minZ = destPosition.z - ((destScale.z / 2) - (resourceDimensions.z / 2));
	float maxZ = destPosition.z + ((destScale.z / 2) - (resourceDimensions.z / 2));

	float resourceX = (rand() % (int)(maxX - minX)) + minX;
	float resourceY = (rand() % (int)(maxY - minY)) + minY;
	float resourceZ = (rand() % (int)(maxZ - minZ)) + minZ;

	ResourcePrefab * resource = new ResourcePrefab(Vec3(resourceX, resourceY, resourceZ),Quaternion::AxisAngleToQuaternion(Vec3(0,0,0),0),Vec3(5,5,5),1000,0.8f,0.4f);
	resource->GetComponent<PhysicsObject*>()->SetLinearVelocity(Vec3(0, -100, 0));
	GameObject::gameWorld->LateInstantiate(resource);
}
