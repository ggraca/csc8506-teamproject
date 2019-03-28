#pragma once
#include "ScriptObject.h"
#include "ResourcePrefab.h"
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
	void DestroyBalls();
	void LateUpdate(float dt) override;
	GameObject* SortPlayers();
	void Aim();
	void Fire();
	
private:
	GameObject* cannon;
	Vector3 posCannon;
	float angleiny;
	float M_PI = 3.14159265358979323846f;
	vector<GameObject*> players;
	vector<GameObject*> balls;
	Vector3 NDiff;
	float timer = 0;
	int ballIndex = 0;

};

