#pragma once
#include "ScriptObject.h"
#include <math.h>


class CannonScript : public ScriptObject
{
public:
public:
	CannonScript(GameObject * gameObject);
	~CannonScript();

	void Awake() override;
	void Start() override;
	void Update(float dt) override;
	void CheckIfDestroyed();
	void LateUpdate(float dt) override;
	//void SetPlayers(GameObject* p1, GameObject* p2, GameObject* p3, GameObject* p4);
	GameObject* SortPlayers();
	void Aim();
	void Fire();
	
private:
	GameObject* cannon;
	GameObject* player1;
	GameObject* player2;
	GameObject* player3;
	GameObject* player4;
	Vector3 posCannon;
	Vector3 pos1;
	Vector3 pos2;
	Vector3 pos3;
	Vector3 pos4;
	int numPlayers;
	float M_PI = 3.14159265358979323846f;
	vector<GameObject*> players;

};

