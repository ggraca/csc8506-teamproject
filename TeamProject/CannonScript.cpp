#include "CannonScript.h"




CannonScript::CannonScript(GameObject * gameObject) : ScriptObject(gameObject)
{
	cannon = gameObject;
	posCannon = cannon->GetTransform().GetWorldPosition();
}


CannonScript::~CannonScript()
{
}

void CannonScript::Awake()
{
}

void CannonScript::Start()
{
}

void CannonScript::Update(float dt)
{
	count = count +1;
	players = GameObject::FindGameObjectsWithTag(LayerAndTag::Tags::Player);
	
	if (players.size() > 0) {
		SortPlayers();
		Aim();
	}
	
	if (count % 100) {
		Fire();
	}

}

void CannonScript::LateUpdate(float dt)
{
}

GameObject* CannonScript::SortPlayers() {
		
		GameObject* closest = nullptr;
		float distance =1000000;

		for (auto i = 0; i < (int)players.size(); i++) {
			if (players[i] != nullptr) {
				Vector3 Diff = players[i]->GetTransform().GetWorldPosition() - posCannon;
				float curDistance = Diff.Length();

				if (curDistance < distance) {
					closest = players[i];
					distance = curDistance;
				}
			}
		}
			return closest;
}

void CannonScript::Aim() {
	
	GameObject * closest = CannonScript::SortPlayers();
	if (!closest) { return; }

	Vector3 Diff = closest->GetTransform().GetWorldPosition() - posCannon;
	NDiff = Diff.Normalised();
	
	float x, y, a,b;

	x = -asin(NDiff.y);
	y = asin(NDiff.x / (cos(asin(x))));
	a = x * (180 / M_PI);
	b = y * (180 / M_PI);

	if (b <-89.5f)
	{
		angleiny = -89.6f;
	}
	if (b > 89.5f)
	{
		angleiny = 89.6f;
	}
	if ((b < 89.5f) && (b > -89.5f)) { angleiny = y * (180 / M_PI); }

	if (closest->GetTransform().GetWorldPosition().z < cannon->GetTransform().GetWorldPosition().z) {
		cannon->GetTransform().ForceUpdateLocalRotationWithTransform(/*Quaternion::AxisAngleToQuaternion(Vector3(1, 0, 0), -a) **/ Quaternion::AxisAngleToQuaternion(Vector3(0, 1, 0), -angleiny - 90));
	}

	if (closest->GetTransform().GetWorldPosition().z > cannon->GetTransform().GetWorldPosition().z) {
		cannon->GetTransform().ForceUpdateLocalRotationWithTransform(/*Quaternion::AxisAngleToQuaternion(Vector3(1, 0, 0), -a) **/ Quaternion::AxisAngleToQuaternion(Vector3(0, 1, 0), angleiny + 90));
	}
}

void CannonScript::Fire() {
	auto cBall = new ResourcePrefab(Vector3(cannon->GetTransform().GetWorldPosition().x, cannon->GetTransform().GetWorldPosition().y +10.0f, cannon->GetTransform().GetWorldPosition().z), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	cBall->GetComponent<DamageControl*>()->SetDamage(4.0f);
	Vector3 projMov = (NDiff * 6000.0f);
	cBall->GetComponent<PhysicsObject*>()->SetLinearVelocity(projMov);
}