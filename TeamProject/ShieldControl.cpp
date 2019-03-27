#include "ShieldControl.h"
#include "PhysicsObject.h"
#include "GameWorld.h"
#include "Player.h"
#include "AudioEngine.h"


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

	GameObject::gameWorld->GetAudio()->PlayEvent("event:/Holy", shield->GetTransform().GetWorldPosition());
}

void ShieldControl::DeactivateShield()
{
	if (!shield) { return; }

	shield->SetActiveStatus(false);

	GameObject::gameWorld->RemoveCollisionsFromGameObject(shield);
	shield->RemoveComponent<PhysicsObject*>();

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
	auto children = gameObject->GetComponent<Player*>()->GetResources();
	int index = 0;

	int resourceAmount = gameObject->GetComponent<Player*>()->GetResourceCount();
	int singleDimension = (int)sqrt(resourceAmount);
	shield->GetTransform().ForceUpdateScale(Vector3(singleDimension * 10.0f, singleDimension * 10.0f, 5.0f));

	for (int i = 0; i < singleDimension; i++)
	{
		for (int j = 0; j < singleDimension; j++)
		{
			GameObject* child = children[index];
			
			// Remove PhysicsObject
			GameObject::gameWorld->RemoveCollisionsFromGameObject(child);
			child->RemoveComponent<PhysicsObject*>();

			// Scale
			child->GetTransform().SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));

			// Position
			Vector3 pos = Vector3(i * 3.0f, j * 3.0f, 0) - Vector3(singleDimension, singleDimension, 0);
			child->GetTransform().SetLocalPosition(pos);
			child->SetParent(shieldDummy);

			child->GetComponent<Resource*>()->SetTarget(nullptr);
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

		Vector3 currentPos = i->GetWorldPosition();
		i->GetGameObject()->GetComponent<Resource*>()->SetTarget(gameObject);
		i->GetGameObject()->SetParent(nullptr);
		i->GetGameObject()->AddComponent<PhysicsObject*>(new PhysicsObject(i, ShapeType::cube, 10));
		i->GetGameObject()->GetTransform().ForceUpdateScaleWithTransform(Vector3(5, 5, 5));
		i->SetWorldPosition(currentPos);

		GameObject::gameWorld->AddObjectPhysicsToWorld(i->GetGameObject()->GetComponent<PhysicsObject*>());
	}
}