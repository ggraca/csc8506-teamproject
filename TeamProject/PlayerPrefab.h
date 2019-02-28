#pragma once
#include "GameObject.h"
#include "Player.h"
#include "CubePrefab.h"

using namespace NCL;
using namespace CSC8503;

class PlayerPrefab : public CubePrefab
{
public:
	PlayerPrefab();
	//TODO parameterized (or whatever it is) which will call parameterized cube prefab construct
	~PlayerPrefab();
};

