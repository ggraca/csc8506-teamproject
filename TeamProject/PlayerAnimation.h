#pragma once
#include "ScriptObject.h"
#include "Animator.h"
#include "Animation.h"

class PlayerAnimation : public ScriptObject
{
public:
	PlayerAnimation(GameObject * obj);
	~PlayerAnimation();
	void Awake() override;


protected:

	GameObject * hammer = nullptr;
	Animator * animator = nullptr;

	void InitializeAnimations();
	void InitializeHammerAnimations();
	static bool HitTransition(GameObject * obj);
	static bool ThirdHitToIdle(GameObject * obj);
	static void ResetHammerHit(GameObject * obj);
};

