#include "CannonScript.h"




CannonScript::CannonScript(GameObject * gameObject) : ScriptObject(gameObject)
{
	cannon = gameObject;
	Vector3 posCannon = cannon->GetTransform().GetWorldPosition();
}


CannonScript::~CannonScript()
{
}

void CannonScript::Awake()
{
	//health = gameObject->GetComponent<HealthManager*>();
	// generate list of all players + distance
	
}

void CannonScript::Start()
{

}

void CannonScript::Update(float dt)
{
	
	players = GameObject::FindGameObjectsWithTag(LayerAndTag::Tags::Player);
	

	CheckIfDestroyed();

	if (players.size() > 0) {
		SortPlayers();
		Aim();
	}
	// check players in range check closers one and fire


}

void CannonScript::CheckIfDestroyed()
{

}

void CannonScript::LateUpdate(float dt)
{
}

//void CannonScript::SetPlayers(GameObject* p1, GameObject* p2, GameObject* p3, GameObject* p4)
//{
//	player1 = p1;
//	numPlayers = 1;
//	if(p2 != nullptr) { player2 = p2; numPlayers = 2; }
//	if (p3 != nullptr) { player3 = p3; numPlayers = 3; }
//	if (p4 != nullptr) { player4 = p4;  numPlayers = 4; }
//}

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
	
	float x, y;

	//x = -asin(Diff.y)/** (180/M_PI)*/;
	//y = asin(Diff.x / (cos(asin(x))))/* * (180 / M_PI)*/;
	y = 90;
	//cout << y << endl;
	//cannon->GetTransform().SetLocalOrientation(Quaternion::AxisAngleToQuaternion(Vector3(1, 0, 0), x) * Quaternion::AxisAngleToQuaternion(Vector3(0, 1, 0), y));
	cannon->GetTransform().SetLocalOrientation(Quaternion::AxisAngleToQuaternion(Vector3(0, 1, 0), y));
}

void CannonScript::Fire() {




}