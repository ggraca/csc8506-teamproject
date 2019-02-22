#include "Destructable.h"
#include "InputManager.h"

Destructable::Destructable(GameObject * gameObject):ScriptObject(gameObject)
{
	//tags set when object is created
}

Destructable::~Destructable()
{
	
}


void Destructable::Awake()
{
}

void Destructable::Start()
{

	switch (gameObject->GetTag)
	{
	case LayerAndTag::SmallWall: objectHealth = 10;
	case LayerAndTag::BigWall: objectHealth = 20;
	case LayerAndTag::Tower: objectHealth = 15;
	case LayerAndTag::Turret: objectHealth = 5;
	}

	
	
}

void Destructable::Update(float dt)
{
	if (objectHealth <= 0)
	{
		
		//create resources
		
		//Let's say it has 100 health, spawn 100/25 = 4;
		

		GameObject * cube = new GameObject();//AddCubeToWorld
		Resource * r = new Resource(cube, inputManager);
		cube->AddScript((ScriptObject*)r);

		//TODO Add it to world
		gameObject->Destroy(gameObject);
	}
}

void Destructable::LateUpdate(float dt)
{
}

void Destructable::OnCollisionBegin(GameObject * otherObject)
{
	if (otherObject->CompareTag(LayerAndTag::Resource))
	{
		objectHealth--;
	}
}


void Destructable::OnCollisionEnd(GameObject * otherObject)
{
}
