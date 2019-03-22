#include "ShieldControl.h"
#include "PhysicsObject.h"
#include "GameWorld.h"
#include "Player.h"

ShieldControl::ShieldControl(GameObject * obj) : ScriptObject(obj)
{
}


ShieldControl::~ShieldControl()
{
}

void ShieldControl::Awake()
{
	DeactivateShield();
}

void ShieldControl::SetTarget(Transform * t)
{
	target = t;
}

void ShieldControl::Update(float dt)
{
	FollowTarget();
}

void ShieldControl::ActivateShield()
{
	if (!shield) { return; }

	shield->SetActiveStatus(true);

	shield->AddComponent<PhysicsObject*>(new PhysicsObject(&shield->GetTransform(), ShapeType::cube, 0));
	GameObject::gameWorld->AddObjectPhysicsToWorld(shield->GetComponent<PhysicsObject*>());

	FormShield();
}

void ShieldControl::DeactivateShield()
{
	if (!shield) { return; }

	shield->SetActiveStatus(false);

	shield->RemoveComponent<PhysicsObject*>();
	GameObject::gameWorld->RemoveCollisionsFromGameObject(shield);

	DeformShield();
}

void ShieldControl::SetShield(GameObject * s)
{
	shield = s;
}

void ShieldControl::SetShieldDummy(GameObject * s)
{
	shieldDummy = s;
}

void ShieldControl::FollowTarget()
{
	if ((!shield || !target) && shield->IsActive()) { return; }

	shield->GetTransform().ForceUpdateWorldPosition(target->GetWorldPosition());
	shield->GetTransform().SetLocalOrientation(target->GetGameObject()->GetTransform().GetWorldOrientation());
}

void ShieldControl::FormShield()
{
	if (!shield || !target || !shieldDummy) { return; }

	//Will be changed
	auto children = GameObject::FindGameObjectsWithTag(gameObject->GetComponent<Player*>()->GetResourceTag());
	int index = 0;

	int resourceAmount = gameObject->GetComponent<Player*>()->GetResourceCount();
	int singleDimension = (int)sqrt(resourceAmount);
	shield->GetTransform().ForceUpdateScale(Vector3(singleDimension * 10.0f, singleDimension * 10.0f, 5.0f ));

	for (int i = 0; i < singleDimension; i++)
	{
		for (int j = 0; j < singleDimension; j++)
		{
			children[index]->GetTransform().SetLocalScale(Vector3(5, 5, 5) / shield->GetTransform().GetLocalScale());
			Vector3 pos = Vector3(i * 3.0f, j * 3.0f, 0) - Vector3(singleDimension, singleDimension,0);
			children[index]->GetComponent<PhysicsObject*>()->GetRigidbody()->clearForces();
			children[index]->GetComponent<Resource*>()->SetTarget(nullptr);
			children[index]->RemoveComponent<PhysicsObject*>();
			GameObject::gameWorld->RemoveCollisionsFromGameObject(children[index]);
			children[index]->GetTransform().SetLocalPosition(pos);
			children[index]->SetParent(shieldDummy);
			index++;
		}
	}
}

void ShieldControl::DeformShield()
{
	if (!shield || !target || !shieldDummy) { return; }
	
	auto children = shieldDummy->GetTransform().GetChildrenList();

	for (auto&i : children)
	{
		if (i->GetGameObject()->GetTag() == LayerAndTag::Tags::HammerHead) { continue; }

		i->GetGameObject()->GetComponent<Resource*>()->SetTarget(gameObject);
		i->GetGameObject()->SetParent(GameObject::FindGameObjectWithTag(LayerAndTag::Tags::CaptureParent));
		i->GetGameObject()->AddComponent<PhysicsObject*>(new PhysicsObject(i, ShapeType::cube, 10));
		i->GetGameObject()->GetTransform().ForceUpdateScaleWithTransform(Vector3(5, 5, 5));

		GameObject::gameWorld->AddObjectPhysicsToWorld(i->GetGameObject()->GetComponent<PhysicsObject*>());
	}
}