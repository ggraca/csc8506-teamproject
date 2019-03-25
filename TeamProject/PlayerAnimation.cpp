#include "PlayerAnimation.h"
#include "HammerControl.h"
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
	animator = gameObject->GetComponent<Animator*>();
	InitializeAnimations();
}

void PlayerAnimation::InitializeAnimations()
{
	if (!animator) { return; }

	InitializeHammerAnimations();
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
	idleHammerKeyFrame->time = 0.2f;

	idleHammerAnimation->AddKeyFrame(idleHammerKeyFrame);

	AnimationState * idleHammerState = new AnimationState(idleHammerAnimation, hammer);
	

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
	idleHammerState->AddTransition(idleHammerToFirstHit);

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
	thirdHitToIdle->destinationState = idleHammerState;
	thirdHitAnimationState->AddTransition(thirdHitToIdle);

	animator->SetDefaultAnimationState(idleHammerState);
	animator->AddAnimationState(idleHammerState);
	animator->AddAnimationState(firstHitAnimationState);
	animator->AddAnimationState(secondHitAnimationState);
	animator->AddAnimationState(thirdHitAnimationState);
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