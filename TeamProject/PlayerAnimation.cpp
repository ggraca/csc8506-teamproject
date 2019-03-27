#include "PlayerAnimation.h"
#include "HammerControl.h"
#include "GunControl.h"
#include "Player.h"

PlayerAnimation::PlayerAnimation(GameObject * obj) : ScriptObject(obj)
{
}


PlayerAnimation::~PlayerAnimation()
{
}

void PlayerAnimation::Awake()
{
	hammer = gameObject->GetComponent<HammerControl*>()->GetHandle();
	leftGun = gameObject->GetComponent<GunControl*>()->GetLeftGun();
	rightGun = gameObject->GetComponent<GunControl*>()->GetRightGun();
	bigGun = gameObject->GetComponent<BigGunControl*>()->GetBigGun();
	animator = gameObject->GetComponent<Animator*>();
	InitializeAnimations();
}

void PlayerAnimation::InitializeAnimations()
{
	if (!animator) { return; }

	InitializeHammerAnimations();
	InitializeGunAnimations();
	InitializeBigGunAnimations();
	SetupIdleTransitions();
}

void PlayerAnimation::InitializeHammerAnimations()
{
	if (!hammer) { return; }

	Transform ht = hammer->GetTransform();
	
	/////Idle/////
	Animation * idleHammerAnimation = new Animation(60);
	
	KeyFrame * idleHammerKeyFrame = new KeyFrame();
	idleHammerKeyFrame->localPosition = ht.GetLocalPosition();
	idleHammerKeyFrame->localRotation = ht.GetLocalOrientation().ToEuler();
	idleHammerKeyFrame->localScale = ht.GetLocalScale();
	idleHammerKeyFrame->time = 0.05f;

	idleHammerAnimation->AddKeyFrame(idleHammerKeyFrame);

	hammerIdleState = new AnimationState(idleHammerAnimation, hammer);
	

	////First Hit////
	Animation * firstHammerHit = new Animation(60);
	
	KeyFrame * firstKeyFrame = new KeyFrame();
	firstKeyFrame->localPosition = Vector3(-2, 0, 1);
	firstKeyFrame->localRotation = Vector3(0, -45, 45);
	firstKeyFrame->localScale = ht.GetLocalScale();
	firstKeyFrame->time = 0.2f;

	KeyFrame * firstKeyFrame1 = new KeyFrame();
	firstKeyFrame1->localPosition = Vector3(-2, 0, 1);
	firstKeyFrame1->localRotation = Vector3(0, -45, 60);
	firstKeyFrame1->localScale = ht.GetLocalScale();
	firstKeyFrame1->time = 0.25f;

	KeyFrame * firstKeyFrame2 = new KeyFrame();
	firstKeyFrame2->localPosition = Vector3(0, 0, 2);
	firstKeyFrame2->localRotation = Vector3(90, 0, 0);
	firstKeyFrame2->localScale = ht.GetLocalScale();
	firstKeyFrame2->time = 0.4f;

	KeyFrame * firstKeyFrame3 = new KeyFrame();
	firstKeyFrame3->localPosition = Vector3(2, 0, 1);
	firstKeyFrame3->localRotation = Vector3(45, 90, -45);
	firstKeyFrame3->localScale = ht.GetLocalScale();
	firstKeyFrame3->time = 0.6f;

	firstHammerHit->AddKeyFrame(firstKeyFrame);
	firstHammerHit->AddKeyFrame(firstKeyFrame1);
	firstHammerHit->AddKeyFrame(firstKeyFrame2);
	firstHammerHit->AddKeyFrame(firstKeyFrame3);

	AnimationState * firstHitAnimationState = new AnimationState(firstHammerHit, hammer);

	////Second Hit////

	Animation * secondHammerHit = new Animation(60);
	
	KeyFrame * secondKeyFrame = new KeyFrame();
	secondKeyFrame->localPosition = Vector3(2, 0, 1);
	secondKeyFrame->localRotation = Vector3(0, 90, -45);
	secondKeyFrame->localScale = ht.GetLocalScale();
	secondKeyFrame->time = 0.2f;

	KeyFrame * secondKeyFrame2 = new KeyFrame();
	secondKeyFrame2->localPosition = Vector3(0, 0,2);
	secondKeyFrame2->localRotation = Vector3(90, 0, 0);
	secondKeyFrame2->localScale = ht.GetLocalScale();
	secondKeyFrame2->time = 0.4f;

	KeyFrame * secondKeyFrame3 = new KeyFrame();
	secondKeyFrame3->localPosition = Vector3(-2, 0, 2);
	secondKeyFrame3->localRotation = Vector3(90, -90, 0);
	secondKeyFrame3->localScale = ht.GetLocalScale();
	secondKeyFrame3->time = 0.6f;

	secondHammerHit->AddKeyFrame(secondKeyFrame);
	secondHammerHit->AddKeyFrame(secondKeyFrame2);
	secondHammerHit->AddKeyFrame(secondKeyFrame3);

	AnimationState * secondHitAnimationState = new AnimationState(secondHammerHit, hammer);

	////Third Hit////

	Animation * thirdHammerHit = new Animation(60);
	
	KeyFrame * thirdKeyFrame = new KeyFrame();
	thirdKeyFrame->localPosition = Vector3(-2, 0, 0);
	thirdKeyFrame->localRotation = Vector3(-90, 45, 0);
	thirdKeyFrame->localScale = ht.GetLocalScale();
	thirdKeyFrame->time = 0.3f;

	KeyFrame * thirdKeyFrame2 = new KeyFrame();
	thirdKeyFrame2->localPosition = Vector3(0, 0, 1);
	thirdKeyFrame2->localRotation = Vector3(90, 45, 0);
	thirdKeyFrame2->localScale = ht.GetLocalScale();
	thirdKeyFrame2->time = 0.6f;

	KeyFrame * thirdKeyFrame3 = new KeyFrame();
	thirdKeyFrame3->localPosition = Vector3(0, 0, 1);
	thirdKeyFrame3->localRotation = Vector3(90, 45, 0);
	thirdKeyFrame3->localScale = ht.GetLocalScale();
	thirdKeyFrame3->time = 0.7f;
	thirdKeyFrame3->keyFunction = PlayerAnimation::ResetHammerHit;

	thirdHammerHit->AddKeyFrame(thirdKeyFrame);
	thirdHammerHit->AddKeyFrame(thirdKeyFrame2);
	thirdHammerHit->AddKeyFrame(thirdKeyFrame3);

	AnimationState * thirdHitAnimationState = new AnimationState(thirdHammerHit, hammer);
	
	////Transitions////
	Transition * idleHammerToFirstHit = new Transition();
	idleHammerToFirstHit->transitionFunction = PlayerAnimation::HitTransition;
	idleHammerToFirstHit->destinationState = firstHitAnimationState;
	hammerIdleState->AddTransition(idleHammerToFirstHit);

	Transition * firstHitToSecondHit = new Transition();
	firstHitToSecondHit->transitionFunction = PlayerAnimation::HitTransition;
	firstHitToSecondHit->destinationState = secondHitAnimationState;
	firstHitAnimationState->AddTransition(firstHitToSecondHit);

	Transition * secondHitToThirdHit = new Transition();
	secondHitToThirdHit->transitionFunction = PlayerAnimation::HitTransition;
	secondHitToThirdHit->destinationState = thirdHitAnimationState;
	secondHitAnimationState->AddTransition(secondHitToThirdHit);

	Transition * thirdHitToIdle = new Transition();
	thirdHitToIdle->transitionFunction = PlayerAnimation::ThirdHitToIdle;
	thirdHitToIdle->destinationState = hammerIdleState;
	thirdHitAnimationState->AddTransition(thirdHitToIdle);

	//animator->SetDefaultAnimationState(hammerIdleState);
	animator->AddAnimationState(hammerIdleState);
	animator->AddAnimationState(firstHitAnimationState);
	animator->AddAnimationState(secondHitAnimationState);
	animator->AddAnimationState(thirdHitAnimationState);
}

void PlayerAnimation::InitializeGunAnimations()
{
	if (!leftGun || !rightGun) { return; }

	Transform rt = rightGun->GetTransform();
	Transform lt = leftGun->GetTransform();

	/////Idles/////
	////Right Idle///
	Animation * idleGunAnimationRight = new Animation(60);

	KeyFrame * idleGunKeyFrameRight = new KeyFrame();
	idleGunKeyFrameRight->localPosition = Vector3(0, 0, 0);
	idleGunKeyFrameRight->localRotation = Vector3(0, 0, 0);
	idleGunKeyFrameRight->localScale = rt.GetLocalScale();
	idleGunKeyFrameRight->time = 0.05f;

	idleGunAnimationRight->AddKeyFrame(idleGunKeyFrameRight);

	gunIdleState = new AnimationState(idleGunAnimationRight, rightGun);

	////////////Left Idle////
	Animation * idleGunAnimationLeft = new Animation(60);

	KeyFrame * idleGunKeyFrameLeft = new KeyFrame();
	idleGunKeyFrameLeft->localPosition = Vector3(0, 0, 0);
	idleGunKeyFrameLeft->localRotation = Vector3(0, 0, 0);
	idleGunKeyFrameLeft->localScale = lt.GetLocalScale();
	idleGunKeyFrameLeft->time = 0.05f;

	idleGunAnimationLeft->AddKeyFrame(idleGunKeyFrameLeft);

	gunIdleStateLeft = new AnimationState(idleGunAnimationLeft, leftGun);

	//Idle Transition (right to left idle)
	Transition * gunIdlesTransition = new Transition();
	gunIdlesTransition->transitionFunction = PlayerAnimation::ExitTransition;
	gunIdlesTransition->destinationState = gunIdleStateLeft;
	gunIdleState->AddTransition(gunIdlesTransition);

	////
	Animation * rightGunActiveAnimation = new Animation(60);

	KeyFrame * rightGunKeyframe = new KeyFrame();
	rightGunKeyframe->localPosition = Vector3(-2, 0, 1);
	rightGunKeyframe->localRotation = Vector3(0, 0, 180);
	rightGunKeyframe->localScale = rt.GetLocalScale();
	rightGunKeyframe->time = 0.1f;

	rightGunActiveAnimation->AddKeyFrame(rightGunKeyframe);

	
	AnimationState * rightGunActivationState = new AnimationState(rightGunActiveAnimation, rightGun);
	////
	Animation * leftGunActiveAnimation = new Animation(60);

	KeyFrame * leftGunKeyframe = new KeyFrame();
	leftGunKeyframe->localPosition = Vector3(2, 0, 1);
	leftGunKeyframe->localRotation = Vector3(0,0,-180);
	leftGunKeyframe->localScale = lt.GetLocalScale();
	leftGunKeyframe->time = 0.1f;

	leftGunActiveAnimation->AddKeyFrame(leftGunKeyframe);

	AnimationState * leftGunActivationState = new AnimationState(leftGunActiveAnimation, leftGun);
	//////
	Transition * activationTransition = new Transition();
	activationTransition->transitionFunction = PlayerAnimation::ExitTransition;
	activationTransition->destinationState = leftGunActivationState;
	rightGunActivationState->AddTransition(activationTransition);

	Transition * idleToActivation = new Transition();
	idleToActivation->transitionFunction = PlayerAnimation::GunActiveTransition;
	idleToActivation->destinationState = rightGunActivationState;
	gunIdleStateLeft->AddTransition(idleToActivation);

	Transition * resetToIdle = new Transition();
	resetToIdle->transitionFunction = PlayerAnimation::GunDeactiveTransition;
	resetToIdle->destinationState = gunIdleState;
	leftGunActivationState->AddTransition(resetToIdle);

	////
	animator->AddAnimationState(gunIdleState);
	animator->AddAnimationState(gunIdleStateLeft);
	animator->AddAnimationState(rightGunActivationState);
	animator->AddAnimationState(leftGunActivationState);
}

void PlayerAnimation::InitializeBigGunAnimations()
{
	if (!bigGun) { return; }

	Transform bgt = bigGun->GetTransform();

	////Idle////
	Animation * idleBigGunAnimation = new Animation(60);

	KeyFrame * idleBigGunFrame = new KeyFrame();
	idleBigGunFrame->localPosition = Vector3(-2,0,-3);
	idleBigGunFrame->localRotation = Vector3(0, 0, 0);
	idleBigGunFrame->localScale = bgt.GetLocalScale();
	idleBigGunFrame->time = 0.1f;

	idleBigGunAnimation->AddKeyFrame(idleBigGunFrame);

	bigGunIdleState = new AnimationState(idleBigGunAnimation, bigGun);

	////Activation////
	Animation * bigGunActivationAnimation = new Animation(60);

	KeyFrame * activeBigGunFrame = new KeyFrame();
	activeBigGunFrame->localPosition = Vector3(-4, 2, 0);
	activeBigGunFrame->localRotation = Vector3( 90, 0, 0);
	activeBigGunFrame->localScale = bgt.GetLocalScale();
	activeBigGunFrame->time = 0.2f;

	bigGunActivationAnimation->AddKeyFrame(activeBigGunFrame);

	AnimationState * bigGunState = new AnimationState(bigGunActivationAnimation, bigGun);

	Transition * activationTransition = new Transition();
	activationTransition->transitionFunction = PlayerAnimation::BigGunActiveTransition;
	activationTransition->destinationState = bigGunState;
	bigGunIdleState->AddTransition(activationTransition);

	Transition * deactivationTransition = new Transition();
	deactivationTransition->transitionFunction = PlayerAnimation::BigGunDeactiveTransition;
	deactivationTransition->destinationState = bigGunIdleState;
	bigGunState->AddTransition(deactivationTransition);
	
	animator->AddAnimationState(bigGunIdleState);
	animator->AddAnimationState(bigGunState);
}

void PlayerAnimation::SetupIdleTransitions()
{
	Transition * hammerIdleToGunIdle = new Transition();
	hammerIdleToGunIdle->transitionFunction = PlayerAnimation::GunActiveTransition;
	hammerIdleToGunIdle->destinationState = gunIdleState;
	hammerIdleState->AddTransition(hammerIdleToGunIdle);

	Transition * gunIdleToHammerIdle = new Transition();
	gunIdleToHammerIdle->transitionFunction = PlayerAnimation::HammerActiveTransition;
	gunIdleToHammerIdle->destinationState = hammerIdleState;
	gunIdleStateLeft->AddTransition(gunIdleToHammerIdle);

	Transition * gunIdleToBigGunIdle = new Transition();
	gunIdleToBigGunIdle->transitionFunction = PlayerAnimation::BigGunActiveTransition;
	gunIdleToBigGunIdle->destinationState = bigGunIdleState;
	gunIdleStateLeft->AddTransition(gunIdleToBigGunIdle);

	Transition * hammerIdleToBigGunIdle = new Transition();
	hammerIdleToBigGunIdle->transitionFunction = PlayerAnimation::BigGunActiveTransition;
	hammerIdleToBigGunIdle->destinationState = bigGunIdleState;
	hammerIdleState->AddTransition(hammerIdleToBigGunIdle);

	Transition * bigGunIdleToGunIdle = new Transition();
	bigGunIdleToGunIdle->transitionFunction = PlayerAnimation::GunActiveTransition;
	bigGunIdleToGunIdle->destinationState = gunIdleState;
	bigGunIdleState->AddTransition(bigGunIdleToGunIdle);

	Transition * bigGunIdleToHammerIdle = new Transition();
	bigGunIdleToHammerIdle->transitionFunction = PlayerAnimation::HammerActiveTransition;
	bigGunIdleToHammerIdle->destinationState = hammerIdleState;
	bigGunIdleState->AddTransition(bigGunIdleToHammerIdle);

}

bool PlayerAnimation::HitTransition(GameObject * obj)
{
	if (obj->GetComponent<Player*>()->IsHammerActive() && 
		obj->GetComponent<Player*>()->GetKeysPressed().inputs[InputManager::ActionButton::HIT] && 
		obj->GetComponent<Animator*>()->GetCurrentAnimationState()->animation->HasAnimationFinished()
		)
	{
		obj->GetComponent<HammerControl*>()->HammerHit();
		return true;
	}
	return false;
}

bool PlayerAnimation::ThirdHitToIdle(GameObject * obj)
{
	if (obj->GetComponent<Animator*>()->GetCurrentAnimationState()->animation->HasAnimationFinished())
	{
		obj->GetComponent<HammerControl*>()->ResetHammerHit();
		return true;
	}
	return false;
}

void PlayerAnimation::ResetHammerHit(GameObject * obj)
{
	return (obj->GetComponent<HammerControl*>()->ResetHammerHit());
}

bool PlayerAnimation::ExitTransition(GameObject * obj)
{
	return (obj->GetComponent<Animator*>()->GetCurrentAnimationState()->animation->HasAnimationFinished());
}

bool PlayerAnimation::GunActiveTransition(GameObject * obj)
{
	return (obj->GetComponent<Player*>()->IsGunActive());
}

bool PlayerAnimation::GunDeactiveTransition(GameObject * obj)
{
	return (!obj->GetComponent<Player*>()->IsGunActive());
}

bool PlayerAnimation::HammerActiveTransition(GameObject * obj)
{
	return (obj->GetComponent<Player*>()->IsHammerActive());
}

bool PlayerAnimation::BigGunActiveTransition(GameObject * obj)
{
	return (obj->GetComponent<Player*>()->IsBugGunActive());
}

bool PlayerAnimation::BigGunDeactiveTransition(GameObject * obj)
{
	return (!obj->GetComponent<Player*>()->IsBugGunActive());
}