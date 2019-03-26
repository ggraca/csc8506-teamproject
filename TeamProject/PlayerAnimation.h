#pragma once
#include "ScriptObject.h"
#include "Animator.h"
#include "Animation.h"
#include "BigGunControl.h"

class PlayerAnimation : public ScriptObject
{
public:
	PlayerAnimation(GameObject * obj);
	~PlayerAnimation();
	void Awake() override;


protected:

	GameObject * hammer = nullptr;
	GameObject * leftGun = nullptr;
	GameObject * rightGun = nullptr;
	GameObject * bigGun = nullptr;
	Animator * animator = nullptr;

	void InitializeAnimations();
	void InitializeHammerAnimations();
	void InitializeGunAnimations();
	void InitializeBigGunAnimations();
	void SetupIdleTransitions();
	static bool HitTransition(GameObject * obj);
	static bool ThirdHitToIdle(GameObject * obj);
	static void ResetHammerHit(GameObject * obj);
	static bool ExitTransition(GameObject * obj);
	static bool GunActiveTransition(GameObject * obj);
	static bool GunDeactiveTransition(GameObject * obj);
	static bool HammerActiveTransition(GameObject * obj);

	static bool BigGunActiveTransition(GameObject * obj);

	static bool BigGunDeactiveTransition(GameObject * obj);

	AnimationState * hammerIdleState = nullptr;
	AnimationState * gunIdleState = nullptr;
	AnimationState * gunIdleStateLeft = nullptr;
	AnimationState * bigGunIdleState = nullptr;
};

