#include "Destructable.h"
#include "InputManager.h"

Destructable::Destructable(GameObject * gameObject):ScriptObject(gameObject)
{
	resourceDimensions = Vector3(5, 5, 5);
}

Destructable::~Destructable()
{
}


void Destructable::Awake()
{
}

void Destructable::Start()
{

}

void Destructable::Update(float dt)
{
	CheckIfDestroyed();
}

void Destructable::CheckIfDestroyed()
{
	//if (1)//dead)
	//{
	//	
	//}
}

void Destructable::LateUpdate(float dt)
{
}

void Destructable::OnCollisionBegin(GameObject * otherObject)
{
	if (otherObject->CompareTag(LayerAndTag::Tags::Player))
	{
		for (int i =0; i < 5;i++)
		{
			GenerateResource();
		}
		GameObject::Destroy(gameObject);
	}
}


void Destructable::OnCollisionEnd(GameObject * otherObject)
{
}

void Destructable::GenerateResource()
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

	/*ExampleScene * es = new ExampleScene();
	GameObject* resource = es->AddCube(Vector3(resourceX, resourceY, resourceZ), Quaternion::EulerAnglesToQuaternion(0, 0, 0), resourceDimensions);*/
	
	resource->AddScript((ScriptObject*)new Resource(resource));
	resource->GetScript<Resource*>()->Reset();

	GameObject::AddObjectToWorld(resource);
}
