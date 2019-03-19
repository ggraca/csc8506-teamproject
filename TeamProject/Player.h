#pragma once
#include "GameObject.h"
#include "Resource.h"

using namespace NCL;
using namespace CSC8503;


class Player : virtual public ScriptObject
{
public:

	Player(GameObject * obj);
	virtual ~Player() {}

	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;
	void UpdateResourceCount(int amount);
	int GetResourceCount() const;
protected:

	void ResetPlayer();
	
	int resourceCount;
};

