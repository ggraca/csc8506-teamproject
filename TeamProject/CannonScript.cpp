#include "CannonScript.h"
#include "FunctionTimer.h"
#include "GameWorld.h"
#include "AudioEngine.h"



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
	timer += dt;
	players = GameObject::FindGameObjectsWithTag(LayerAndTag::Tags::Player);
	
	if (players.size() > 0) {
		SortPlayers();
		Aim();
	}
	
	if (((float)(rand() % 1000) / 1000) < 0.15f * dt) {
		Fire();
	}

	//if (timer > 10.0f)
	//{
	//	timer = 0;
	//	DestroyBalls();
	//}
}

void CannonScript::DestroyBalls()
{
	for (auto&i : balls)
	{
		GameObject::gameWorld->RemoveCollisionsFromGameObject(i);
		GameObject::gameWorld->LateDestroy(i);
	}
	balls.clear();
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

void CannonScript::Fire()
{

	GameObject * closest = CannonScript::SortPlayers();
	if (!closest) { return; }
	Vector3 Diff = closest->GetTransform().GetWorldPosition() - posCannon;
	NDiff = Diff.Normalised();

	if (balls.size() == 20)
	{
		if (balls[ballIndex]->GetComponent<PhysicsObject*>())
		{
			balls[ballIndex]->GetComponent<PhysicsObject*>()->SetLinearVelocity(Vector3(0, 0, 0));
			balls[ballIndex]->GetTransform().ForceUpdateWorldPositionWithTransform(Vector3(cannon->GetTransform().GetWorldPosition().x, cannon->GetTransform().GetWorldPosition().y + 50.0f, cannon->GetTransform().GetWorldPosition().z));
			balls[ballIndex]->SetTag(LayerAndTag::EnemyProjectile);
			balls[ballIndex]->GetComponent<DamageControl*>()->SetDamage(4.0f);
			Vector3 projMov = (cannon->GetTransform().GetWorldPosition() + (NDiff * 5000)) - Vector3(0, 50, 0);
			balls[ballIndex]->GetComponent<PhysicsObject*>()->SetLinearVelocity(projMov);

			ballIndex = (ballIndex + 1) % 20;
		}
	}
	else
	{
		auto cBall = new ResourcePrefab(Vector3(cannon->GetTransform().GetWorldPosition().x, cannon->GetTransform().GetWorldPosition().y + 50.0f, cannon->GetTransform().GetWorldPosition().z), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
		cBall->SetTag(LayerAndTag::EnemyProjectile);
		cBall->GetComponent<DamageControl*>()->SetDamage(4.0f);
		Vector3 projMov = (cannon->GetTransform().GetWorldPosition() - Vector3(0, 50, 0) + (NDiff * 5000)) ;
		if (cBall->GetComponent<PhysicsObject*>()) { cBall->GetComponent<PhysicsObject*>()->SetLinearVelocity(projMov); }
		GameObject::gameWorld->LateInstantiate(cBall);
		balls.push_back(cBall);
	}
	
	GameObject::gameWorld->GetAudio()->PlayEvent("event:/gun", cannon->GetTransform().GetWorldPosition());
}